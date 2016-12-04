; Function Attrs: norecurse nounwind sspstrong uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @free(i8* inttoptr (i64 1 to i8*)) #2
  ret i32 0
}

; Function Attrs: nounwind
declare void @free(i8*) #1
