; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

define i32 @addition(i32 %n, i32 %m) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %m2 = alloca i32, align 4
  store i32 %m, i32* %m2, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %n3 = load i32, i32* %n1, align 4
  %m4 = load i32, i32* %m2, align 4
  %int_add = add i32 %n3, %m4
  store i32 %int_add, i32* %result, align 4
  %n5 = load i32, i32* %n1, align 4
  %int_eq = icmp eq i32 %n5, 4
  br i1 %int_eq, label %then, label %else

then:                                             ; preds = %function_body
  %n6 = load i32, i32* %n1, align 4
  %m7 = load i32, i32* %m2, align 4
  %int_add8 = add i32 %n6, %m7
  %func_call = call i32 @print_int(i32 %int_add8)
  br label %after

else:                                             ; preds = %function_body
  %n9 = load i32, i32* %n1, align 4
  %m10 = load i32, i32* %m2, align 4
  %int_mul = mul i32 %n9, %m10
  %func_call11 = call i32 @print_int(i32 %int_mul)
  br label %after

after:                                            ; preds = %else, %then
  %result12 = load i32, i32* %result, align 4
  ret i32 %result12
  ret i32 0
}
