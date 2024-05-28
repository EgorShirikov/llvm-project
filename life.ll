; ModuleID = 'life.cpp'
source_filename = "life.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx13.0.0"


define dso_local i32 @_Z14calcNeighboursPiii(i32* nocapture readonly %0, i32 %1, i32 %2) local_unnamed_addr #0 {
  %4 = add nsw i32 %1, -1
  %5 = add nsw i32 %2, -1
  br label %7

6:                                                ; preds = %13
  ret i32 %31

7:                                                ; preds = %3, %13
  %8 = phi i32 [ %4, %3 ], [ %14, %13 ]
  %9 = phi i32 [ 0, %3 ], [ %31, %13 ]
  %10 = icmp ugt i32 %8, 59
  %11 = icmp eq i32 %8, %1
  %12 = mul nsw i32 %8, 80
  br label %16

13:                                               ; preds = %30
  %14 = add nsw i32 %8, 1
  %15 = icmp sgt i32 %8, %1
  br i1 %15, label %6, label %7, !llvm.loop !7

16:                                               ; preds = %7, %30
  %17 = phi i32 [ %5, %7 ], [ %32, %30 ]
  %18 = phi i32 [ %9, %7 ], [ %31, %30 ]
  %19 = icmp ugt i32 %17, 79
  %20 = or i1 %10, %19
  %21 = icmp eq i32 %17, %2
  %22 = and i1 %11, %21
  %23 = or i1 %20, %22
  br i1 %23, label %30, label %24

24:                                               ; preds = %16
  %25 = add nsw i32 %17, %12
  %26 = zext i32 %25 to i64
  %27 = getelementptr inbounds i32, i32* %0, i64 %26
  %28 = load i32, i32* %27, align 4, !tbaa !10
  %29 = add nsw i32 %28, %18
  br label %30

30:                                               ; preds = %16, %24
  %31 = phi i32 [ %29, %24 ], [ %18, %16 ]
  %32 = add nsw i32 %17, 1
  %33 = icmp sgt i32 %17, %2
  br i1 %33, label %13, label %16, !llvm.loop !14
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree norecurse nounwind ssp uwtable mustprogress
define dso_local void @_Z7calcGenPiS_(i32* nocapture readonly %0, i32* nocapture %1) local_unnamed_addr #2 {
  br label %4

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %2, %4
  %5 = phi i64 [ 0, %2 ], [ %20, %4 ]
  %6 = trunc i64 %5 to i32
  %7 = udiv i32 %6, 80
  %8 = trunc i64 %5 to i32
  %9 = urem i32 %8, 80
  %10 = call i32 @_Z14calcNeighboursPiii(i32* %0, i32 %7, i32 %9)
  %11 = getelementptr inbounds i32, i32* %0, i64 %5
  %12 = load i32, i32* %11, align 4, !tbaa !10
  %13 = icmp eq i32 %12, 0
  %14 = and i32 %10, -2
  %15 = icmp eq i32 %14, 2
  %16 = icmp eq i32 %10, 3
  %17 = select i1 %13, i1 %16, i1 %15
  %18 = zext i1 %17 to i32
  %19 = getelementptr inbounds i32, i32* %1, i64 %5
  store i32 %18, i32* %19, align 4, !tbaa !10
  %20 = add nuw nsw i64 %5, 1
  %21 = icmp eq i64 %20, 4800
  br i1 %21, label %3, label %4, !llvm.loop !15
}

; Function Attrs: ssp uwtable mustprogress
define dso_local void @_Z7drawGenPi(i32* nocapture readonly %0) local_unnamed_addr #3 {
  br label %3

2:                                                ; preds = %3
  tail call void @llvm.may.flush()
  ret void

3:                                                ; preds = %1, %3
  %4 = phi i64 [ 0, %1 ], [ %10, %3 ]
  %5 = getelementptr inbounds i32, i32* %0, i64 %4
  %6 = load i32, i32* %5, align 4, !tbaa !10
  %7 = mul nsw i32 %6, 65280
  %8 = add i32 %7, -16777216
  %9 = trunc i64 %4 to i32
  tail call void @llvm.may.setPixel(i32 %9, i32 %8)
  %10 = add nuw nsw i64 %4, 1
  %11 = icmp eq i64 %10, 4800
  br i1 %11, label %2, label %3, !llvm.loop !16
}

declare void @llvm.may.setpixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #3


; Function Attrs: ssp uwtable mustprogress
define dso_local void @_Z7initGenPi(i32* nocapture %0) local_unnamed_addr #3 {
  br label %3

2:                                                ; preds = %3
  ret void

3:                                                ; preds = %1, %3
  %4 = phi i64 [ 0, %1 ], [ %7, %3 ]
  %5 = tail call i32 @llvm.may.rand()
  %6 = getelementptr inbounds i32, i32* %0, i64 %4
  store i32 %5, i32* %6, align 4, !tbaa !10
  %7 = add nuw nsw i64 %4, 1
  %8 = icmp eq i64 %7, 4800
  br i1 %8, label %2, label %3, !llvm.loop !17
}

declare void @llvm.may.rand() local_unnamed_addr #4

; Function Attrs: norecurse ssp uwtable mustprogress
define dso_local i32 @main() local_unnamed_addr #5 {
  %1 = alloca [4800 x i32], align 4
  %2 = alloca [4800 x i32], align 4
  %3 = bitcast [4800 x i32]* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 19200, i8* nonnull %3) #6
  %4 = bitcast [4800 x i32]* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 19200, i8* nonnull %4) #6
  %5 = getelementptr inbounds [4800 x i32], [4800 x i32]* %2, i64 0, i64 0
  tail call void @llvm.may.event()
  call void @_Z7initGenPi(i32* nonnull %5)
  %6 = call zeroext i1 tail call void @llvm.may.event()
  br i1 %6, label %7, label %13

7:                                                ; preds = %0
  %8 = getelementptr inbounds [4800 x i32], [4800 x i32]* %1, i64 0, i64 0
  br label %9

9:                                                ; preds = %7, %9
  %10 = phi i32* [ %11, %9 ], [ %8, %7 ]
  %11 = phi i32* [ %10, %9 ], [ %5, %7 ]
  call void @_Z7calcGenPiS_(i32* %11, i32* %10)
  call void @_Z7drawGenPi(i32* %11)
  %12 = tail call zeroext i1 void @llvm.may.event()
  br i1 %12, label %9, label %13, !llvm.loop !18

13:                                               ; preds = %9, %0
  tail call void @llvm.may.exit()
  call void @llvm.lifetime.end.p0i8(i64 19200, i8* nonnull %4) #6
  call void @llvm.lifetime.end.p0i8(i64 19200, i8* nonnull %3) #6
  ret i32 0
}

declare void @llvm.may.flush() local_unnamed_addr #4

declare void @llvm.may.event() local_unnamed_addr #4

declare void @llvm.may.exit() local_unnamed_addr #4


!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"branch-target-enforcement", i32 0}
!2 = !{i32 1, !"sign-return-address", i32 0}
!3 = !{i32 1, !"sign-return-address-all", i32 0}
!4 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{!"Homebrew clang version 12.0.1"}
!7 = distinct !{!7, !8, !9}
!8 = !{!"llvm.loop.mustprogress"}
!9 = !{!"llvm.loop.unroll.disable"}
!10 = !{!11, !11, i64 0}
!11 = !{!"int", !12, i64 0}
!12 = !{!"omnipotent char", !13, i64 0}
!13 = !{!"Simple C++ TBAA"}
!14 = distinct !{!14, !8, !9}
!15 = distinct !{!15, !8, !9}
!16 = distinct !{!16, !8, !9}
!17 = distinct !{!17, !8, !9}
!18 = distinct !{!18, !8, !9}
