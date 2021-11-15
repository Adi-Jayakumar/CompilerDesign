; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define void @Void() {
function_body:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %result, align 4
  %result1 = load i32, i32* %result, align 4
  %func_call = call i32 @print_int(i32 %result1)
  br label %loop_condition

loop_condition:                                   ; preds = %after_block, %function_body
  %result2 = load i32, i32* %result, align 4
  %int_lt = icmp slt i32 %result2, 10
  br i1 %int_lt, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  br label %block_body

block_body:                                       ; preds = %loop_body
  %result3 = load i32, i32* %result, align 4
  %int_add = add i32 %result3, 1
  store i32 %int_add, i32* %result, align 4
  %result4 = load i32, i32* %result, align 4
  %func_call5 = call i32 @print_int(i32 %result4)
  br label %after_block

after_block:                                      ; preds = %block_body
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  ret void
  ret void
}
