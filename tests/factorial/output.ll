; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @factorial(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  %factorial = alloca i32, align 4
  store i32 0, i32* %factorial, align 4
  store i32 1, i32* %factorial, align 4
  store i32 1, i32* %i, align 4
  %factorial2 = load i32, i32* %factorial, align 4
  ret i32 %factorial2
  ret i32 0
}
