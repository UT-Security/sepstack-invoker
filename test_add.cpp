#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

static unsigned int add(unsigned int a1, unsigned int a2) {
  return a1 + a2;
}

TEST_CASE("Simple function", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    unsigned int a = 34;
    unsigned int b = 57;

    auto target_func_ptr = &add;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a, b);

    REQUIRE(ret == a+b);
}
