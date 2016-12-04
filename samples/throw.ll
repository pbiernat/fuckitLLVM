@_ZTIi = external constant i8*

; Function Attrs: norecurse sspstrong uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %2 = call i8* @__cxa_allocate_exception(i64 4) #1
  %3 = bitcast i8* %2 to i32*
  store i32 1, i32* %3, align 16
  call void @__cxa_throw(i8* %2, i8* bitcast (i8** @_ZTIi to i8*), i8* null) #2
  unreachable
                                                  ; No predecessors!
  %5 = load i32, i32* %1, align 4
  ret i32 %5
}

declare i8* @__cxa_allocate_exception(i64)

declare void @__cxa_throw(i8*, i8*, i8*)
