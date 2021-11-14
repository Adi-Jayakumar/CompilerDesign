; ModuleID = 'mini-c'
source_filename = "mini-c"

define i32 @multiplyNumbers(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 0, i32* %result, align 4
  %n2 = load i32, i32* %n1, align 4
  %int_geq = icmp sge i32 %n2, 1
  br i1 %int_geq, label %then, label %else

then:                                             ; preds = %function_body
  %n3 = load i32, i32* %n1, align 4
  %n4 = load i32, i32* %n1, align 4
  %int_sub = sub i32 %n4, 1
  %func_call = call i32 @multiplyNumbers(i32 %int_sub)
  %int_mul = mul i32 %n3, %func_call
  store i32 %int_mul, i32* %result, align 4
  br label %after

else:                                             ; preds = %function_body
  store i32 1, i32* %result, align 4
  br label %after

after:                                            ; preds = %else, %then
  %result5 = load i32, i32* %result, align 4
  ret i32 %result5
  ret i32 0
}

define i32 @rfact(i32 %n) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %n2 = load i32, i32* %n1, align 4
  %func_call = call i32 @multiplyNumbers(i32 %n2)
  ret i32 %func_call
  ret i32 0
}
