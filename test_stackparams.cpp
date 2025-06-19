#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

static unsigned int stackParametersTest(unsigned int a1, unsigned int a2,
                                           unsigned int a3, unsigned int a4,
                                           unsigned int a5, unsigned int a6,
                                           unsigned int a7, unsigned int a8,
                                           unsigned int a9, unsigned int a10,
                                           unsigned int a11) {
  return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11;
}

TEST_CASE("Function with stack params", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    auto target_func_ptr = &stackParametersTest;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

    REQUIRE(ret == 1+2+3+4+5+6+7+8+9+10+11);
}
