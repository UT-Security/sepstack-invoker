#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

struct largeStruct {
  uint64_t a1;
  uint64_t a2;
  uint64_t a3;
};

static largeStruct testLargeStructReturn() {
  largeStruct ret;
  ret.a1 = 3;
  ret.a2 = 7;
  ret.a3 = 9;
  return ret;
}

TEST_CASE("Function with large struct return", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    auto target_func_ptr = &testLargeStructReturn;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr));

    REQUIRE(ret.a1 == 3);
    REQUIRE(ret.a2 == 7);
    REQUIRE(ret.a3 == 9);
}
