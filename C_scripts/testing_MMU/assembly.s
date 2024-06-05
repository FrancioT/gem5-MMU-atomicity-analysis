.global data_test
.func data_test
.type data_test, %function

data_test:
  MOV x3, #0

r:LSL x4, x3, #12
  ADD x4, x0, x4
  ADD x4, x4, #4
  LDR x4, [x4]

  ADD x3, x3, #1
  CMP x3, x1
  
  BNE not_equal
  MOV x3, #0
  
not_equal:
  SUBS x2, x2, #1
  BNE r

  RET LR
.endfunc

