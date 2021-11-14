; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @nothing() {
function_body:
  %x = alloca i32, align 4
  store i32 0, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %int_to_bool = trunc i32 %x1 to i1
  br i1 %int_to_bool, label %then, label %else

then:                                             ; preds = %function_body
  ret i32 1
  br label %after

else:                                             ; preds = %function_body
  ret i32 2
  br label %after

after:                                            ; preds = %else, %then
  ret i32 0
}
