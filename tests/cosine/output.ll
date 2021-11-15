; ModuleID = 'mini-c'
source_filename = "mini-c"

declare float @print_float(float)

define float @cosine(float %x) {
function_body:
  %x1 = alloca float, align 4
  store float %x, float* %x1, align 4
  %cos = alloca float, align 4
  store float 0.000000e+00, float* %cos, align 4
  %n = alloca float, align 4
  store float 0.000000e+00, float* %n, align 4
  %term = alloca float, align 4
  store float 0.000000e+00, float* %term, align 4
  %eps = alloca float, align 4
  store float 0.000000e+00, float* %eps, align 4
  %alt = alloca float, align 4
  store float 0.000000e+00, float* %alt, align 4
  store float 0x3EB0C6F7A0000000, float* %eps, align 4
  store float 1.000000e+00, float* %n, align 4
  store float 1.000000e+00, float* %cos, align 4
  store float 1.000000e+00, float* %term, align 4
  store float -1.000000e+00, float* %alt, align 4
  br label %loop_condition

loop_condition:                                   ; preds = %after_block, %function_body
  %term2 = load float, float* %term, align 4
  %eps3 = load float, float* %eps, align 4
  %float_gt = fcmp ugt float %term2, %eps3
  br i1 %float_gt, label %loop_body, label %after_loop

loop_body:                                        ; preds = %loop_condition
  br label %block_body

block_body:                                       ; preds = %loop_body
  %term4 = load float, float* %term, align 4
  %x5 = load float, float* %x1, align 4
  %float_mul = fmul float %term4, %x5
  %x6 = load float, float* %x1, align 4
  %float_mul7 = fmul float %float_mul, %x6
  %n8 = load float, float* %n, align 4
  %float_div = fdiv float %float_mul7, %n8
  %n9 = load float, float* %n, align 4
  %float_add = fadd float %n9, 1.000000e+00
  %float_div10 = fdiv float %float_div, %float_add
  store float %float_div10, float* %term, align 4
  %cos11 = load float, float* %cos, align 4
  %alt12 = load float, float* %alt, align 4
  %term13 = load float, float* %term, align 4
  %float_mul14 = fmul float %alt12, %term13
  %float_add15 = fadd float %cos11, %float_mul14
  store float %float_add15, float* %cos, align 4
  %alt16 = load float, float* %alt, align 4
  %f_unary_neg = fsub float 0.000000e+00, %alt16
  store float %f_unary_neg, float* %alt, align 4
  %n17 = load float, float* %n, align 4
  %float_add18 = fadd float %n17, 2.000000e+00
  store float %float_add18, float* %n, align 4
  br label %after_block

after_block:                                      ; preds = %block_body
  br label %loop_condition

after_loop:                                       ; preds = %loop_condition
  %cos19 = load float, float* %cos, align 4
  %func_call = call float @print_float(float %cos19)
  %cos20 = load float, float* %cos, align 4
  ret float %cos20
  ret float 0.000000e+00
}
