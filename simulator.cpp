//  CPU INSTRUCTION FORMATS:
//  Registers: 16 registers (32 bits) R0-R15
//
//  FF FF FF FF
//        ^^ ^^ - R3/IMM - third register or immediate
//      ^       - R2     - second register
//     ^        - R1     - first register
//  ^^          - opcode - operation code

#include "iostream"
#include "elfio/elfio.hpp"
#include "../Conway-Game-Of-Life/sim.h"

enum Opcode : unsigned {
  OPCODE_SUB = 0x31,
  OPCODE_ADD = 0x30,
  OPCODE_MUL = 0x32,
  OPCODE_DIV = 0x33,
  OPCODE_OR = 0x34,
  OPCODE_AND = 0x35,
  OPCODE_XOR = 0x36,
  OPCODE_SHL = 0x37,
  OPCODE_SRL = 0x38,
  OPCODE_ADDI = 0x40,
  OPCODE_MULI = 0x42,
  OPCODE_ORI = 0x44,
  OPCODE_BEQ = 0x70,
  OPCODE_BNE = 0x71,
  OPCODE_BLE = 0x72,
  OPCODE_BGE = 0x71,
  OPCODE_MOVLI = 0xAA,
  OPCODE_MOVHI = 0xAF,
  OPCODE_PUTPIXEL = 0x70,
  OPCODE_FLUSH = 0xF1,
  OPCODE_INIT = 0xF2,
  OPCODE_EXIT = 0xF3
};


using namespace ELFIO;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <elf_file>" << std::endl;
    return 1;
  }

  elfio reader;

  if (!reader.load(argv[1])) {
    std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
    return 2;
  }

  std::cout << "ELF file class : ";
  if ( reader.get_class() == ELFCLASS32 )
    std::cout << "ELF32" << std::endl;
  else
    std::cout << "ELF64" << std::endl;

  std::cout << "ELF file encoding : ";
  if ( reader.get_encoding() == ELFDATA2LSB )
    std::cout << "Little endian" << std::endl;
  else
    std::cout << "Big endian" << std::endl;

  Elf_Half sec_num = reader.sections.size();
  const uint32_t* code = nullptr;
  Elf_Xword code_size = 0;
  Elf_Xword code_start = 0;
  for (size_t i = 0; i < sec_num; ++i) {
    section* psec = reader.sections[i];
    if (psec->get_name() == ".text") {
      code = reinterpret_cast<const uint32_t*>(reader.sections[i]->get_data());
      code_size = psec->get_size() / 4;
      std::cout << "  [" << i << "] " << psec->get_name() << "\t"
                << psec->get_size() << std::endl;
    }
  }

  for (size_t i = 0; i < sec_num; ++i ) {
    section* psec = reader.sections[i];
    if (psec->get_type() == SHT_SYMTAB ) {
      const symbol_section_accessor symbols( reader, psec );
      for (size_t j = 0; j < symbols.get_symbols_num(); ++j ) {
        std::string   name;
        Elf64_Addr    value;
        Elf_Xword     size;
        unsigned char bind;
        unsigned char type;
        Elf_Half      section_index;
        unsigned char other;

        symbols.get_symbol( j, name, value, size, bind, type,
                           section_index, other );
        if (name == "main") {
          code_start = value;
          std::cout << j << " " << name << " " << value << std::endl;
        }
      }
    }
  }

  if (code) {
    std::cout << "[CODE] SIZE:" << code_size << "(x4) START:" << code_start << std::endl;
    for (int i = 0; i < code_size; ++i) {
      printf("%04x: %08x%s\n", i, code[i], i == code_start ? "<=" : "");
    }
    const int REG_FILE_SIZE = 16;
    uint32_t REG_FILE[REG_FILE_SIZE] = {};
    REG_FILE[0] = code_size + 1;
    uint32_t PC = code_start / 4;
    uint32_t NEXT_PC = PC + 1;
    uint32_t RUN = 1;

    while (RUN) {
      const uint8_t r2 = (code[PC] >> 16) & 0xF;
      const uint8_t r1 = (code[PC] >> 20) & 0xF;
      const uint16_t r3_imm = code[PC] & 0xFFFF;
      const enum Opcode opcode = static_cast<Opcode>(code[PC] >> 24);

      printf("[%04x] ", PC);
      switch (opcode) {
      case OPCODE_MOVLI:
        printf("MOVli r%d %x\n", r1, r3_imm);
        REG_FILE[r1] = r3_imm;
        break;
      case OPCODE_MOVHI:
        printf("MOVhi r%d %x\n", r1, r3_imm << 16);
        REG_FILE[r1] = r3_imm << 16;
        break;
      case OPCODE_BNE:
        printf("BNE r%d r%d %x\n", r1, r2, PC + (int16_t)r3_imm);
        if (REG_FILE[r1] != REG_FILE[r2]) {
          NEXT_PC = PC + (int16_t)r3_imm;
        }
        break;
      case OPCODE_ADD:
        printf("ADD r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] + REG_FILE[r3_imm];
        break;
      case OPCODE_MUL:
        printf("MUL r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] * REG_FILE[r3_imm];
        break;
      case OPCODE_DIV:
        printf("DIV r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] * REG_FILE[r3_imm];
        break;
      case OPCODE_OR:
        printf("OR r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] | REG_FILE[r3_imm];
        break;
      case OPCODE_AND:
        printf("AND r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] & REG_FILE[r3_imm];
        break;
      case OPCODE_XOR:
        printf("XOR r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] & REG_FILE[r3_imm];
        break;
      case OPCODE_SHL:
        printf("SHL r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] << REG_FILE[r3_imm];
        break;
      case OPCODE_SRL:
        printf("SRL r%d r%d r%d\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] >> REG_FILE[r3_imm];
        break;
      case OPCODE_ADDI:
        printf("ADDi r%d r%d %x\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] + (int16_t)r3_imm;
        break;
      case OPCODE_ORI:
        printf("ORi r%d r%d %x\n", r1, r2, r3_imm);
        REG_FILE[r1] = REG_FILE[r2] | r3_imm;
        break;
      case OPCODE_BEQ:
        printf("BEQ r%d r%d %x\n", r1, r2, PC + (int16_t)r3_imm);
        if (REG_FILE[r1] == REG_FILE[r2]) {
          NEXT_PC = PC + (int16_t)r3_imm;
        }
        break;
      case OPCODE_BLE:
        printf("BLE r%d r%d %x\n", r1, r2, PC + (int16_t)r3_imm);
        if (REG_FILE[r1] <= REG_FILE[r2]) {
          NEXT_PC = PC + (int16_t)r3_imm;
        }
        break;
      case OPCODE_BGE:
        printf("BGE r%d r%d %x\n", r1, r2, PC + (int16_t)r3_imm);
        if (REG_FILE[r1] >= REG_FILE[r2]) {
          NEXT_PC = PC + (int16_t)r3_imm;
        }
        break;
      case OPCODE_PUTPIXEL:
        printf("PUTPIXEL (%d, %d): %x\n", REG_FILE[r1], REG_FILE[r2], REG_FILE[r3_imm]);
        simSetPixel(REG_FILE[r1], REG_FILE[r2], REG_FILE[r3_imm]);
        break;
      case OPCODE_FLUSH:
        // 0xFF FLUSH
        printf("FLUSH\n");
        simFlush();
        break;
      case OPCODE_EXIT:
        // 0xFF FLUSH
        printf("FLUSH\n");
        simExit();
      case OPCODE_INIT:
        // 0xFF FLUSH
        printf("FLUSH\n");
        simExit();
        break;
      default:
        printf("[ERROR] PC %04x: Incorect opcode %02x\n", PC, op);
        RUN = 0;
        break;
      }
      PC = NEXT_PC;
      NEXT_PC = PC + 1;
      if (0 > PC || PC > code_size) {
        RUN = 0;
      }
    }
    return REG_FILE[9];
  }
  return 0;
}