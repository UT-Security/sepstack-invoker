#include "sepstack_ctx_offsets.h"

.text

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

	// Clear non param reg except the tmp reg r11
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

.section .note.GNU-stack,"",@progbits
