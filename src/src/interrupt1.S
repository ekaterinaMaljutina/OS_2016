.text
.extern handler
.extern iqr_handler

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
    pushq $1
    jmp handler_stub


.global handler2
handler2:
    subq $8, %rsp
    pushq $2
    jmp handler_stub


.global handler3
handler3:
    subq $8, %rsp
    pushq $3
    jmp handler_stub


.global handler4
handler4:
    subq $8, %rsp
    pushq $8
    jmp handler_stub



.global handler5
handler5:
    subq $8, %rsp
    pushq $5
    jmp handler_stub


.global handler6
handler6:
    subq $8, %rsp
    pushq $6
    jmp handler_stub



.global handler7
handler7:
    subq $8, %rsp
    pushq $7
    jmp handler_stub

.global handler8
handler8: #код ошибки есть на стеке
    subq $8, %rsp
    pushq $8
    jmp handler_stub

.global handler9
handler9: 
    subq $8, %rsp
    pushq $9
    jmp handler_stub

.global handler10
handler10: #код ошибки есть на стеке
    pushq $10
    jmp handler_stub

.global handler11
handler11: 
    pushq $11
    jmp handler_stub

.global handler12
handler12: 
    pushq $12
    jmp handler_stub


.global handler13
handler13: 
    pushq $13
    jmp handler_stub

.global handler14
handler14: 
    pushq $14
    jmp handler_stub

.global handler15
handler15: 
    subq $8, %rsp
    pushq $15
    jmp handler_stub

.global handler16
handler16: 
    subq $8, %rsp
    pushq $16
    jmp handler_stub

.global handler17
handler17: 
    subq $8, %rsp
    pushq $17
    jmp handler_stub

.global handler18
handler18: 
    subq $8, %rsp
    pushq $18
    jmp handler_stub

.global handler19
handler19: 
    subq $8, %rsp
    pushq $19
    jmp handler_stub


.global handler20
handler20: 
    subq $8, %rsp
    pushq $20
    jmp handler_stub

.global handler21
handler21: 
    subq $8, %rsp
    pushq $21
    jmp handler_stub

.global handler22
handler22: 
    subq $8, %rsp
    pushq $22
    jmp handler_stub

.global handler23
handler23: 
    subq $8, %rsp
    pushq $23
    jmp handler_stub

.global handler24
handler24:
    subq $8, %rsp
    pushq $24
    jmp handler_stub

.global handler25
handler25: 
    subq $8, %rsp
    pushq $25
    jmp handler_stub


.global handler26
handler26: 
    subq $8, %rsp
    pushq $26
    jmp handler_stub


.global handler27
handler27: 
    subq $8, %rsp
    pushq $27
    jmp handler_stub



.global handler28
handler28: 
    subq $8, %rsp
    pushq $28
    jmp handler_stub

.global handler29
handler29: 
    subq $8, %rsp
    pushq $29
    jmp handler_stub


.global handler30
handler30: 
    subq $8, %rsp
    jmp handler_stub

.global handler31
handler31: 
    subq $8, %rsp
    jmp handler_stub


#PIC 32 to 47 !!!! 
#.extern handler31

handler_irq:
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
    call irq_handler # in C code
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
    addq $0x8, %rsp
    iretq


.global handler32
handler32:
    pushq $32
    jmp handler_irq


.global handler33
handler33:
    pushq $33
    jmp handler_irq


.global handler34
handler34:
    pushq $34
    jmp handler_irq


.global handler35
handler35:
    pushq $35
    jmp handler_irq

.global handler36
handler36:
    pushq $36
    jmp handler_irq

.global handler37
handler37:
    pushq $37
    jmp handler_irq



.global handler38
handler38:
    pushq $38
    jmp handler_irq

.global handler39
handler39:
    pushq $39
    jmp handler_irq



.global handler40
handler40:
    pushq $40
    jmp handler_irq


.global handler41
handler41:
    pushq $41
    jmp handler_irq

.global handler42
handler42:
    pushq $42
    jmp handler_irq

.global handler43
handler43:
    pushq $43
    jmp handler_irq

.global handler44
handler44:
    pushq $44
    jmp handler_irq

.global handler45
handler45:
    pushq $45
    jmp handler_irq

.global handler46
handler46:
    pushq $46
    jmp handler_irq

.global handler47
handler47:
    pushq $47
    jmp handler_irq