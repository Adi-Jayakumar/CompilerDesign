; ModuleID = 'mini-c'
source_filename = "mini-c"

declare float @print_float(float)

define float @cosine(float %x) {
function_body:
  %x1 = alloca float, align 4
  store float %x, float* %x1, align 4
  %cos = alloca float, align 4
  store float 0.000000e+00, float* %cos, align 8
  %n = alloca float, align 4
  store float 0.000000e+00, float* %n, align 8
  %term = alloca float, align 4
  store float 0.000000e+00, float* %term, align 8
  %eps = alloca float, align 4
  store float 0.000000e+00, float* %eps, align 8
  %alt = alloca float, align 4
  store float 0.000000e+00, float* %alt, align 8
  store float 0x3EB0C6F7A0000000, float* %eps, align 8
  store float 1.000000e+00, float* %n, align 8
  store float 1.000000e+00, float* %cos, align 8
  store float 1.000000e+00, float* %term, align 8
  store float sub (float 0.000000e+00, float 1.000000e+00), float* %alt, align 8
  %cos2 = load float, float* %cos, align 4
  %func_call = call float @print_float(float %cos2)
  %cos3 = load float, float* %cos, align 4
  ret float %cos3
  ret float 0.000000e+00
}
