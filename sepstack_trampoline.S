#include "sepstack_ctx_offsets.h"

.text

#if defined(__x86_64__) || defined(_M_X64)

.global trampoline_stack_change
trampoline_stack_change:
	endbr64

	// save callee-saved registers
	pushq %rbx
	pushq %rbp
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

	// Load context
	movq saved_sepstack_context@gottpoff(%rip), %r11
	movq %fs:(%r11), %r11

	// Clear non param registers, except the tmp reg r11
	xorq %rax, %rax
	xorq %rbx, %rbx
	xorq %rbp, %rbp
	xorq %r10, %r10
	xorq %r12, %r12
	xorq %r13, %r13
	xorq %r14, %r14
	xorq %r15, %r15
	// Clear direction flag before call or return
	cld
	// Clear the x87, MMX, and SSE state.
	fxrstor fxrstor_default_state(%rip)
	// Clear the AVX state that the "fxrstor" instruction doesn't cover.
	vzeroupper

	// Load params
	movq SEPSTACK_CONTEXT_OFFSET_RDI(%r11), %rdi
	movq SEPSTACK_CONTEXT_OFFSET_RSI(%r11), %rsi
	movq SEPSTACK_CONTEXT_OFFSET_RDX(%r11), %rdx
	movq SEPSTACK_CONTEXT_OFFSET_RCX(%r11), %rcx
	movq SEPSTACK_CONTEXT_OFFSET_R8(%r11), %r8
	movq SEPSTACK_CONTEXT_OFFSET_R9(%r11), %r9
	movsd SEPSTACK_CONTEXT_OFFSET_XMM0(%r11), %xmm0
	movsd SEPSTACK_CONTEXT_OFFSET_XMM1(%r11), %xmm1
	movsd SEPSTACK_CONTEXT_OFFSET_XMM2(%r11), %xmm2
	movsd SEPSTACK_CONTEXT_OFFSET_XMM3(%r11), %xmm3
	movsd SEPSTACK_CONTEXT_OFFSET_XMM4(%r11), %xmm4
	movsd SEPSTACK_CONTEXT_OFFSET_XMM5(%r11), %xmm5
	movsd SEPSTACK_CONTEXT_OFFSET_XMM6(%r11), %xmm6
	movsd SEPSTACK_CONTEXT_OFFSET_XMM7(%r11), %xmm7

	// save old rsp and load new rsp
	// save this to context as this value is needed for nested calls
	movq %rsp, SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR(%r11)
	movq SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR(%r11), %rsp

	// load address of the target function and invoke
	movq SEPSTACK_CONTEXT_OFFSET_TARGET_PROG_CTR(%r11), %r11
	callq *%r11

	// Load context
	movq saved_sepstack_context@gottpoff(%rip), %r11
	movq %fs:(%r11), %r11

	// restore old stack
	movq SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR(%r11), %rsp

	// Save return registers
	movq  %rax, SEPSTACK_CONTEXT_OFFSET_RAX(%r11)
	movq  %rdx, SEPSTACK_CONTEXT_OFFSET_RDX(%r11)
	movsd  %xmm0, SEPSTACK_CONTEXT_OFFSET_XMM0(%r11)
	movsd  %xmm1, SEPSTACK_CONTEXT_OFFSET_XMM1(%r11)

	// Clear direction flag before call or return
	cld
	// Clear the x87, MMX, and SSE state.
	fxrstor fxrstor_default_state(%rip)

	// restore callee-saved registers
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %rbp
	popq %rbx
	ret

.global springboard_stack_change
springboard_stack_change:
	endbr64

	// Load context
	movq saved_sepstack_context@gottpoff(%rip), %r11
	movq %fs:(%r11), %r11

	// Save params
	movq %rdi  , SEPSTACK_CONTEXT_OFFSET_RDI(%r11)
	movq %rsi  , SEPSTACK_CONTEXT_OFFSET_RSI(%r11)
	movq %rdx  , SEPSTACK_CONTEXT_OFFSET_RDX(%r11)
	movq %rcx  , SEPSTACK_CONTEXT_OFFSET_RCX(%r11)
	movq %r8   , SEPSTACK_CONTEXT_OFFSET_R8(%r11)
	movq %r9   , SEPSTACK_CONTEXT_OFFSET_R9(%r11)
	movsd %xmm0, SEPSTACK_CONTEXT_OFFSET_XMM0(%r11)
	movsd %xmm1, SEPSTACK_CONTEXT_OFFSET_XMM1(%r11)
	movsd %xmm2, SEPSTACK_CONTEXT_OFFSET_XMM2(%r11)
	movsd %xmm3, SEPSTACK_CONTEXT_OFFSET_XMM3(%r11)
	movsd %xmm4, SEPSTACK_CONTEXT_OFFSET_XMM4(%r11)
	movsd %xmm5, SEPSTACK_CONTEXT_OFFSET_XMM5(%r11)
	movsd %xmm6, SEPSTACK_CONTEXT_OFFSET_XMM6(%r11)
	movsd %xmm7, SEPSTACK_CONTEXT_OFFSET_XMM7(%r11)

	// Clear direction flag before call or return
	cld
	// Clear the x87, MMX, and SSE state.
	fxrstor fxrstor_default_state(%rip)

	// save old rsp and load new rsp
	// save this to context as this value is needed for nested calls
	movq %rsp, SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR(%r11)
	movq SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR(%r11), %rsp

	// save context which will also align the stack
	pushq %r11

	// load address of the target function and invoke
	movq callback_func@gottpoff(%rip), %r11
	movq %fs:(%r11), %r11
	callq *%r11

	// restore context
	popq %r11

	// Zero non-callee save registers except r11 and return registers
	xorq %rcx, %rcx
	xorq %rsi, %rsi
	xorq %rdi, %rdi
	xorq %r8,  %r8
	xorq %r9,  %r9
	xorq %r10, %r10

	// Clear direction flag before call or return
	cld
	// Clear the x87, MMX, and SSE state.
	fxrstor fxrstor_default_state(%rip)
	// Clear the AVX state that the "fxrstor" instruction doesn't cover.
	vzeroupper

	// Load return registers
	movq SEPSTACK_CONTEXT_OFFSET_RAX(%r11), %rax
	movq SEPSTACK_CONTEXT_OFFSET_RDX(%r11), %rdx
	movsd SEPSTACK_CONTEXT_OFFSET_XMM0(%r11), %xmm0
	movsd SEPSTACK_CONTEXT_OFFSET_XMM1(%r11), %xmm1

	// Restore the sandbox stack
	movq SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR(%r11), %rsp

	ret

.section .rodata, "a"
/*
This is the memory block for "fxrstor" to read.  The only contents that matter
are the fcw and mxcsr words. The mxcsr_mask word is ignored by the hardware, so
there is no need to get the hardware-supplied value for that.  The hardware
requires that this address be aligned to 16 bytes.  Align it further to 64 bytes
to get potential cache benefits.
*/
        .balign 64
fxrstor_default_state:
         .byte 0x7f // fcw
         .byte 0x03 // fcw
        .space 22
        .byte 0x80 // mxcsr
        .byte 0x1f // mxcsr
        .space 486


#elif defined(__aarch64__) || defined(_M_ARM64)

.global trampoline_stack_change
trampoline_stack_change:
	bti jc

	// save callee-saved registers
	stp d8, d9,   [sp, #-16]!
	stp d10, d11, [sp, #-16]!
	stp d12, d13, [sp, #-16]!
	stp d14, d15, [sp, #-16]!
	stp x19, x20, [sp, #-16]!
	stp x21, x22, [sp, #-16]!
	stp x23, x24, [sp, #-16]!
	stp x25, x26, [sp, #-16]!
	stp x27, x28, [sp, #-16]!
	stp x29, x30, [sp, #-16]!

	// Load context
	mrs x16, tpidr_el0
	adrp x0, :tlsdesc:saved_sepstack_context
	ldr x17, [x0, :tlsdesc_lo12:saved_sepstack_context]
	add x0, x0, :tlsdesc_lo12:saved_sepstack_context
	.tlsdesccall saved_sepstack_context
	blr x17
	ldr x17, [x16, x0]

	// Clear non param registers, except the tmp reg x16, x17
	mov x9, #0
	mov x10, #0
	mov x11, #0
	mov x12, #0
	mov x13, #0
	mov x14, #0
	mov x15, #0
	mov x16, #0

	// Load params
	ldr x0, [x17, SEPSTACK_CONTEXT_OFFSET_X0]
	ldr x1, [x17, SEPSTACK_CONTEXT_OFFSET_X1]
	ldr x2, [x17, SEPSTACK_CONTEXT_OFFSET_X2]
	ldr x3, [x17, SEPSTACK_CONTEXT_OFFSET_X3]
	ldr x4, [x17, SEPSTACK_CONTEXT_OFFSET_X4]
	ldr x5, [x17, SEPSTACK_CONTEXT_OFFSET_X5]
	ldr x6, [x17, SEPSTACK_CONTEXT_OFFSET_X6]
	ldr x7, [x17, SEPSTACK_CONTEXT_OFFSET_X7]
	ldr d0, [x17, SEPSTACK_CONTEXT_OFFSET_Q0]
	ldr d1, [x17, SEPSTACK_CONTEXT_OFFSET_Q1]
	ldr d2, [x17, SEPSTACK_CONTEXT_OFFSET_Q2]
	ldr d3, [x17, SEPSTACK_CONTEXT_OFFSET_Q3]
	ldr d4, [x17, SEPSTACK_CONTEXT_OFFSET_Q4]
	ldr d5, [x17, SEPSTACK_CONTEXT_OFFSET_Q5]
	ldr d6, [x17, SEPSTACK_CONTEXT_OFFSET_Q6]
	ldr d7, [x17, SEPSTACK_CONTEXT_OFFSET_Q7]

	// save old rsp and load new rsp
	// save this to context as this value is needed for nested calls
	mov x16, sp
	str x16, [x17, SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR]
	ldr x16, [x17, SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR]
	mov sp, x16

	// clear x16
	mov x16, #0

	// load address of the target function and invoke
	ldr x17, [x17, SEPSTACK_CONTEXT_OFFSET_TARGET_PROG_CTR]
	blr x17

	// Save x0
	mov x19, x0

	// Load context
	mrs x16, tpidr_el0
	adrp x0, :tlsdesc:saved_sepstack_context
	ldr x17, [x0, :tlsdesc_lo12:saved_sepstack_context]
	add x0, x0, :tlsdesc_lo12:saved_sepstack_context
	.tlsdesccall saved_sepstack_context
	blr x17
	ldr x17, [x16, x0]

	// Restore x0
	mov x0, x19

	// restore old stack
	ldr x16, [x17, SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR]
	mov sp, x16

	// Save return registers
	str  x0, [x17, SEPSTACK_CONTEXT_OFFSET_X0]
	str  x1, [x17, SEPSTACK_CONTEXT_OFFSET_X1]
	// Save floating point
	str  d0, [x17, SEPSTACK_CONTEXT_OFFSET_Q0]
	str  d1, [x17, SEPSTACK_CONTEXT_OFFSET_Q1]

	// restore callee-saved registers
	ldp x29, x30, [sp], 16
	ldp x27, x28, [sp], 16
	ldp x25, x26, [sp], 16
	ldp x23, x24, [sp], 16
	ldp x21, x22, [sp], 16
	ldp x19, x20, [sp], 16
	ldp d14, d15, [sp], 16
	ldp d12, d13, [sp], 16
	ldp d10, d11, [sp], 16
	ldp d8, d9,   [sp], 16
	br x30

.global springboard_stack_change
springboard_stack_change:
	.inst 0x0000dead

#else
#error "Unknown architecture"
#endif


.section .note.GNU-stack,"",@progbits
