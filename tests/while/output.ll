; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @While(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %test = alloca i32, align 4
  store i32 0, i32* %test, align 4
  %f = alloca float, align 4
  store float 0.000000e+00, float* %f, align 4
  %b = alloca i1, align 1
  store i1 false, i1* %b, align 1
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 12, i32* %test, align 4
  store i32 0, i32* %result, align 4
  %test2 = load i32, i32* %test, align 4
  %func_call = call i32 @print_int(i32 %test2)
  br label %loop_condition

loop_condition:                                   ; preds = %loop_body, %function_body
  %result3 = load i32, i32* %result, align 4
  %int_lt = icmp slt i32 %result3, 10
  br i1 %int_lt, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  %result4 = load i32, i32* %result, align 4
  %int_add = add i32 %result4, 1
  store i32 %int_add, i32* %result, align 4
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  %result5 = load i32, i32* %result, align 4
  ret i32 %result5
  ret i32 0
}
