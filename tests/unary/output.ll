; ModuleID = 'mini-c'
source_filename = "mini-c"

declare i32 @print_int(i32)

declare float @print_float(float)

define float @unary(i32 %n, float %m) {
function_body:
  %n1 = alloca i32, align 4
  store i32 %n, i32* %n1, align 4
  %m2 = alloca float, align 4
  store float %m, float* %m2, align 4
  %result = alloca float, align 4
  store float 0.000000e+00, float* %result, align 4
  %sum = alloca float, align 4
  store float 0.000000e+00, float* %sum, align 4
  store float 0.000000e+00, float* %sum, align 4
  %n3 = load i32, i32* %n1, align 4
  %int_to_float = sitofp i32 %n3 to float
  %m4 = load float, float* %m2, align 4
  %float_add = fadd float %int_to_float, %m4
  store float %float_add, float* %result, align 4
  %result5 = load float, float* %result, align 4
  %func_call = call float @print_float(float %result5)
  %sum6 = load float, float* %sum, align 4
  %result7 = load float, float* %result, align 4
  %float_add8 = fadd float %sum6, %result7
  store float %float_add8, float* %sum, align 4
  %n9 = load i32, i32* %n1, align 4
  %int_to_float10 = sitofp i32 %n9 to float
  %m11 = load float, float* %m2, align 4
  %f_unary_neg = fsub float 0.000000e+00, %m11
  %float_add12 = fadd float %int_to_float10, %f_unary_neg
  store float %float_add12, float* %result, align 4
  %result13 = load float, float* %result, align 4
  %func_call14 = call float @print_float(float %result13)
  %sum15 = load float, float* %sum, align 4
  %result16 = load float, float* %result, align 4
  %float_add17 = fadd float %sum15, %result16
  store float %float_add17, float* %sum, align 4
  %n18 = load i32, i32* %n1, align 4
  %int_to_float19 = sitofp i32 %n18 to float
  %m20 = load float, float* %m2, align 4
  %f_unary_neg21 = fsub float 0.000000e+00, %m20
  %f_unary_neg22 = fsub float 0.000000e+00, %f_unary_neg21
  %float_add23 = fadd float %int_to_float19, %f_unary_neg22
  store float %float_add23, float* %result, align 4
  %result24 = load float, float* %result, align 4
  %func_call25 = call float @print_float(float %result24)
  %sum26 = load float, float* %sum, align 4
  %result27 = load float, float* %result, align 4
  %float_add28 = fadd float %sum26, %result27
  store float %float_add28, float* %sum, align 4
  %n29 = load i32, i32* %n1, align 4
  %i_unary_neg = sub i32 0, %n29
  %int_to_float30 = sitofp i32 %i_unary_neg to float
  %m31 = load float, float* %m2, align 4
  %f_unary_neg32 = fsub float 0.000000e+00, %m31
  %float_add33 = fadd float %int_to_float30, %f_unary_neg32
  store float %float_add33, float* %result, align 4
  %result34 = load float, float* %result, align 4
  %func_call35 = call float @print_float(float %result34)
  %sum36 = load float, float* %sum, align 4
  %result37 = load float, float* %result, align 4
  %float_add38 = fadd float %sum36, %result37
  store float %float_add38, float* %sum, align 4
  %sum39 = load float, float* %sum, align 4
  ret float %sum39
  ret float 0.000000e+00
}
