; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @fibonacci(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %first = alloca i32, align 4
  store i32 0, i32* %first, align 4
  %second = alloca i32, align 4
  store i32 0, i32* %second, align 4
  %next = alloca i32, align 4
  store i32 0, i32* %next, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %c, align 4
  %total = alloca i32, align 4
  store i32 0, i32* %total, align 4
  %n2 = load i32, i32* %n1, align 4
  %func_call = call i32 @print_int(i32 %n2)
  store i32 0, i32* %first, align 4
  store i32 1, i32* %second, align 4
  store i32 1, i32* %c, align 4
  store i32 0, i32* %total, align 4
  br label %loop_condition

loop_condition:                                   ; preds = %after_block, %function_body
  %c3 = load i32, i32* %c, align 4
  %n4 = load i32, i32* %n1, align 4
  %int_lt = icmp slt i32 %c3, %n4
  br i1 %int_lt, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  br label %block_body

block_body:                                       ; preds = %loop_body
  %c5 = load i32, i32* %c, align 4
  %int_leq = icmp sle i32 %c5, 1
  br i1 %int_leq, label %then, label %else

then:                                             ; preds = %block_body
  %c6 = load i32, i32* %c, align 4
  store i32 %c6, i32* %next, align 4
  br label %after

else:                                             ; preds = %block_body
  %first7 = load i32, i32* %first, align 4
  %second8 = load i32, i32* %second, align 4
  %int_add = add i32 %first7, %second8
  store i32 %int_add, i32* %next, align 4
  %second9 = load i32, i32* %second, align 4
  store i32 %second9, i32* %first, align 4
  %next10 = load i32, i32* %next, align 4
  store i32 %next10, i32* %second, align 4
  br label %after

after:                                            ; preds = %else, %then
  %next11 = load i32, i32* %next, align 4
  %func_call12 = call i32 @print_int(i32 %next11)
  %c13 = load i32, i32* %c, align 4
  %int_add14 = add i32 %c13, 1
  store i32 %int_add14, i32* %c, align 4
  %total15 = load i32, i32* %total, align 4
  %next16 = load i32, i32* %next, align 4
  %int_add17 = add i32 %total15, %next16
  store i32 %int_add17, i32* %total, align 4
  br label %after_block

after_block:                                      ; preds = %after
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  %total18 = load i32, i32* %total, align 4
  %func_call19 = call i32 @print_int(i32 %total18)
  %total20 = load i32, i32* %total, align 4
  ret i32 %total20
  ret i32 0
}
