; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @addNumbers(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %result, align 4
  %n2 = load i32, i32* %n1, align 4
  %int_neq = icmp ne i32 %n2, 0
  br i1 %int_neq, label %then, label %else

then:                                             ; preds = %function_body
  %n3 = load i32, i32* %n1, align 4
  %n4 = load i32, i32* %n1, align 4
  %int_sub = sub i32 %n4, 1
  %func_call = call i32 @addNumbers(i32 %int_sub)
  %int_add = add i32 %n3, %func_call
  store i32 %int_add, i32* %result, align 4
  br label %after

else:                                             ; preds = %function_body
  %n5 = load i32, i32* %n1, align 4
  store i32 %n5, i32* %result, align 4
  br label %after

after:                                            ; preds = %else, %then
  %result6 = load i32, i32* %result, align 4
  %func_call7 = call i32 @print_int(i32 %result6)
  %result8 = load i32, i32* %result, align 4
  ret i32 %result8
  ret i32 0
}

define i32 @recursion_driver(i32 %num) {
function_body:
  %num1 = alloca i32, align 4
  store i32 %num, i32* %num1, align 4
  %num2 = load i32, i32* %num1, align 4
  %func_call = call i32 @addNumbers(i32 %num2)
  ret i32 %func_call
  ret i32 0
}
