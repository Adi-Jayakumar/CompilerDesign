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
  br label %loop_condition

loop_condition:                                   ; preds = %after_block, %function_body
  %i2 = load i32, i32* %i, align 4
  %n3 = load i32, i32* %n1, align 4
  %int_leq = icmp sle i32 %i2, %n3
  br i1 %int_leq, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  br label %block_body

block_body:                                       ; preds = %loop_body
  %factorial4 = load i32, i32* %factorial, align 4
  %i5 = load i32, i32* %i, align 4
  %int_mul = mul i32 %factorial4, %i5
  store i32 %int_mul, i32* %factorial, align 4
  %i6 = load i32, i32* %i, align 4
  %int_add = add i32 %i6, 1
  store i32 %int_add, i32* %i, align 4
  br label %after_block

after_block:                                      ; preds = %block_body
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  %factorial7 = load i32, i32* %factorial, align 4
  ret i32 %factorial7
  ret i32 0
}
