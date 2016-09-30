.text
.extern handler

handler_stub:
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq    %rsp, %rdi
    cld
    call handler
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    addq $0x10, %rsp
    iretq



.global handler0
handler0:
    subq $8, %rsp
    pushq $0
    jmp handler_stub


.global handler1
handler1:
    subq $8, %rsp
    pushq $0
    jmp handler_stub


.global handler2
handler2:
    subq $8, %rsp
    pushq $0
    jmp handler_stub


.global handler3
handler3:
    subq $8, %rsp
    pushq $0
    jmp handler_stub


.global handler4
handler4:
    subq $8, %rsp
    pushq $0
    jmp handler_stub



.global handler5
handler5:
    subq $8, %rsp
    pushq $0
    jmp handler_stub


.global handler6
handler6:
    subq $8, %rsp
    pushq $0
    jmp handler_stub



.global handler7
handler7:
    subq $8, %rsp
    pushq $0
    jmp handler_stub

.global handler8
handler8: #код ошибки есть на стеке
    subq $8, %rsp
    pushq $0
    jmp handler_stub
