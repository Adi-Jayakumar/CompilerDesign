; ModuleID = 'mini-c'
source_filename = "mini-c"

declare float @print_float(float)

define void @nothing() {
function_body:
  %func_call = call float @print_float(float 0x400921FB60000000)
  ret void
}
