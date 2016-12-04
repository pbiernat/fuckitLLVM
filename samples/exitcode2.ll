; Function Attrs: norecurse nounwind sspstrong uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @exit(i32 1) #2
  unreachable
                                                  ; No predecessors!
  %3 = load i32, i32* %1, align 4
  ret i32 %3
}

; Function Attrs: noreturn nounwind
declare void @exit(i32) #1
