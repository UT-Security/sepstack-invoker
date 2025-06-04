#pragma once

#include <stdint.h>
#include "sepstack_ctx_offsets.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sepstack_context_t;

#ifdef __cplusplus
}

#define CHECK_FIELD_OFFSET(offset_val, field)                                  \
  static_assert(offset_val == __builtin_offsetof(sepstack_context_t, field));

#else
#define CHECK_FIELD_OFFSET(offset_val, field)
#endif


/////////////////////////////////////////

#if defined(__x86_64__)

typedef struct sepstack_context_t {
  /*          0x0, 0x8, 0x10, 0x18, 0x20, 0x28 */
  uint64_t    rax, rbx,  rcx,  rdx,  rsi,  rdi;
  /*          0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68 */
  uint64_t    r8,     r9,  r10,  r11,  r12,  r13,  r14,  r15;
  /*          0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xa0, 0xa8 */
  uint64_t    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
  /*          0xb0, 0xb8,  0xc0,  0xc8,  0xd0,  0xd8,  0xe0,  0xe8 */
  uint64_t    xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
  /*          0xf0,                         0xf8,            0x100,          0x108 */
  uint64_t    source_frame_ptr, source_stack_ptr, target_frame_ptr, target_stack_ptr;
  /*          0x110,                     0x118 */
  uint64_t    source_prog_ctr, target_prog_ctr;
  /*          0x120,               0x128 */
  uint64_t    source_mxcsr, target_mxcsr; // these are 32-bit, but 64-bit simplifies offsets
  /*          0x130,           0x138 */
  uint64_t    source_fcw, target_fcw; // these are 16-bit, but 64-bit simplifies offsets
  /* end:     0x140 */
} sepstack_context_t;

CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RAX, rax);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RBX, rbx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RCX, rcx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RDX, rdx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RSI, rsi);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_RDI, rdi);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R8, r8);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R9, r9);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R10, r10);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R11, r11);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R12, r12);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R13, r13);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R14, r14);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_R15, r15);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM0, xmm0);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM1, xmm1);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM2, xmm2);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM3, xmm3);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM4, xmm4);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM5, xmm5);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM6, xmm6);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM7, xmm7);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM8, xmm8);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM9, xmm9);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM10, xmm10);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM11, xmm11);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM12, xmm12);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM13, xmm13);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM14, xmm14);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_XMM15, xmm15);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_FRAME_PTR, source_frame_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR, source_stack_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_FRAME_PTR, target_frame_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR, target_stack_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_PROG_CTR, source_prog_ctr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_PROG_CTR, target_prog_ctr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_MXCSR, source_mxcsr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_MXCSR, target_mxcsr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_FCW, source_fcw);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_FCW, target_fcw);

/////////////////////////////////////////

#elif defined(__i386__)

typedef struct sepstack_context_t {
  /*          0x0, 0x4, 0x8, 0xc, 0x10, 0x14 */
  uint32_t    eax, ebx,  ecx,  edx,  esi,  edi;
  /*          0x18 */
  uint64_t    st0;
  /*          0x20,                         0x24,             0x28,             0x2c */
  uint32_t    source_frame_ptr, source_stack_ptr, target_frame_ptr, target_stack_ptr;
  /*          0x30,                        0x34 */
  uint32_t    source_prog_ctr, target_prog_ctr;
  /*          0x38,                 0x3c */
  uint32_t    source_mxcsr, target_mxcsr;
  /*          0x40,             0x44 */
  uint32_t    source_fcw, target_fcw; // these are 16-bit, but 32-bit simplifies offsets
  /* end:     0x48 */
} sepstack_context_t;

CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_EAX, eax);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_EBX, ebx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_ECX, ecx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_EDX, edx);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_ESI, esi);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_EDI, edi);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_ST0, st0);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_FRAME_PTR, source_frame_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_STACK_PTR, source_stack_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_FRAME_PTR, target_frame_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_STACK_PTR, target_stack_ptr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_PROG_CTR, source_prog_ctr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_PROG_CTR, target_prog_ctr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_MXCSR, source_mxcsr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_MXCSR, target_mxcsr);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_SOURCE_FCW, source_fcw);
CHECK_FIELD_OFFSET(SEPSTACK_CONTEXT_OFFSET_TARGET_FCW, target_fcw);

#else
#error "Unknown architecture"
#endif

#if !defined(__ASSEMBLER__) && defined(__cplusplus)
static_assert(SEPSTACK_CONTEXT_TOTAL_SIZE == sizeof(sepstack_context_t));
#undef CHECK_FIELD_OFFSET
#endif
