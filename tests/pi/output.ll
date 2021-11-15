; ModuleID = 'mini-c'
source_filename = "mini-c"

define float @pi() {
function_body:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  %PI = alloca float, align 4
  store float 0.000000e+00, float* %PI, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i1 true, i1* %flag, align 1
  store float 3.000000e+00, float* %PI, align 4
  store i32 2, i32* %i, align 4
  br label %loop_condition

loop_condition:                                   ; preds = %after_block, %function_body
  %i1 = load i32, i32* %i, align 4
  %int_lt = icmp slt i32 %i1, 100
  br i1 %int_lt, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  br label %block_body

block_body:                                       ; preds = %loop_body
  %flag2 = load i1, i1* %flag, align 1
  br i1 %flag2, label %then, label %else

then:                                             ; preds = %block_body
  %PI3 = load float, float* %PI, align 4
  %i4 = load i32, i32* %i, align 4
  %i5 = load i32, i32* %i, align 4
  %int_add = add i32 %i5, 1
  %int_mul = mul i32 %i4, %int_add
  %i6 = load i32, i32* %i, align 4
  %int_add7 = add i32 %i6, 2
  %int_mul8 = mul i32 %int_mul, %int_add7
  %int_to_float = sitofp i32 %int_mul8 to float
  %float_div = fdiv float 4.000000e+00, %int_to_float
  %float_add = fadd float %PI3, %float_div
  store float %float_add, float* %PI, align 4
  br label %after

else:                                             ; preds = %block_body
  %PI9 = load float, float* %PI, align 4
  %i10 = load i32, i32* %i, align 4
  %i11 = load i32, i32* %i, align 4
  %int_add12 = add i32 %i11, 1
  %int_mul13 = mul i32 %i10, %int_add12
  %i14 = load i32, i32* %i, align 4
  %int_add15 = add i32 %i14, 2
  %int_mul16 = mul i32 %int_mul13, %int_add15
  %int_to_float17 = sitofp i32 %int_mul16 to float
  %float_div18 = fdiv float 4.000000e+00, %int_to_float17
  %float_sub = fsub float %PI9, %float_div18
  store float %float_sub, float* %PI, align 4
  br label %after

after:                                            ; preds = %else, %then
  %flag19 = load i1, i1* %flag, align 1
  %b_not = xor i1 %flag19, true
  store i1 %b_not, i1* %flag, align 1
  %i20 = load i32, i32* %i, align 4
  %int_add21 = add i32 %i20, 2
  store i32 %int_add21, i32* %i, align 4
  br label %after_block

after_block:                                      ; preds = %after
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  %PI22 = load float, float* %PI, align 4
  ret float %PI22
  ret float 0.000000e+00
}
