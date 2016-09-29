.text
.extern handler

handler_stub:
	pushq %eax
    pushq %ebx
    pushq %ecx
    pushq %edx
    pushq %esi
    pushq %edi
    pushq %ebp
    pushq %rg_8
    pushq %rg_9
    pushq %rg_10
    pushq %rg_11
    pushq %rg_12
    pushq %rg_13
    pushq %rg_14
    pushq %rg_15
	movq	%esp, %edi
    cld
    call handler
    popq %rg_15
    popq %rg_14
    popq %rg_13
    popq %rg_12
    popq %rg_11
    popq %rg_10
    popq %rg_9
    popq %rg_8
    popq %ebp
    popq %edi
    popq %esi
    popq %edx
    popq %ecx
    popq %ebx
    popq %eax
	addq $0x10, %esp
    iretq



.global handler0
handler0:
	subq $8, %esp
	pushq $0
	jmp handler_stub