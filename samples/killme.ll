; Function Attrs: norecurse nounwind sspstrong uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %2

; <label>:2:                                      ; preds = %0, %2
  br label %2
                                                  ; No predecessors!
  %4 = load i32, i32* %1, align 4
  ret i32 %4
}
