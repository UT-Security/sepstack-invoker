#pragma once

#include <stdio.h>
#include <memory>

extern "C" {
    __attribute__((weak)) extern thread_local void *callback_func;
    extern void *springboard_stack_change;
}

template <typename... TArgs>
inline void printTypes()
{
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
  puts(__PRETTY_FUNCTION__); // NOLINT
#elif defined(_MSC_VER)
  puts(__FUNCSIG__); // NOLINT
#else
  puts("Unsupported");
#endif
}

std::unique_ptr<char[]> get_stack_allocation(uintptr_t* out_sbx_stack_end);
