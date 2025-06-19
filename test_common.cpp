#include <memory>

#include "test_common.h"


extern "C" {
__attribute__((weak)) thread_local void *callback_func = 0;
}

std::unique_ptr<char[]> get_stack_allocation(uintptr_t* out_sbx_stack_end) {
  auto sbx_stack_alloc = std::make_unique<char[]>(2 * 1024 * 1024);
  *out_sbx_stack_end =
      reinterpret_cast<uintptr_t>(sbx_stack_alloc.get()) + 2 * 1024 * 1024;

  *out_sbx_stack_end -= *out_sbx_stack_end % 32; // mod 32 = 0
  *out_sbx_stack_end -= 8;                       // mod 32 = 8

  return sbx_stack_alloc;
}
