.text
.code64

.extern thread_origin

.global thread_origin_
.global switch_threads

thread_origin_:
  movq %r15, %rdi
  movq %r14, %rsi
  
  cld
  callq thread_origin

switch_threads:
  pushq %rbp
  pushq %rbx
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15
  pushfq

  movq %rsp, (%rdi)
  movq %rsi, %rsp

  popfq
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %rbx
  popq %rbp

  ret