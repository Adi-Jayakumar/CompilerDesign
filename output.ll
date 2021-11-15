; ModuleID = 'mini-c'
source_filename = "mini-c"

declare float @print_float(float)

define i32 @nothing() {
function_body:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %j, align 4
  store i32 17, i32* %i, align 4
  store i32 3, i32* %j, align 4
  %i1 = load i32, i32* %i, align 4
  %j2 = load i32, i32* %j, align 4
  %i_mod = srem i32 %i1, %j2
  ret i32 %i_mod
  ret i32 0
}
