; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @nothing(i32 %x, i32 %y) {
function_body:
  %x1 = alloca i32, align 4
  store i32 %x, i32* %x1, align 4
  %y2 = alloca i32, align 4
  store i32 %y, i32* %y2, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %z, align 4
  store i32 5, i32* %x1, align 4
  store i32 10, i32* %z, align 4
  %x3 = load i32, i32* %x1, align 4
  %z4 = load i32, i32* %z, align 4
  %int_add = add i32 %x3, %z4
  ret i32 %int_add
}
