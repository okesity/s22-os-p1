
# timer handler asm entry
.global timer_asm_handler
timer_asm_handler:
  pusha
  call timer_c_handler
  popa
  iret

# keyboard handler asm entry
.global keyboard_asm_handler
keyboard_asm_handler:
  pusha
  call keyboard_c_handler
  popa
  iret

