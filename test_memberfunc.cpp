#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

class zog {
public:
  uint64_t val;

  zog(uint64_t s) : val(s) {}

  int member_func(int p) { return val + p; }

  int const_member_func(int p) const { return val + p; }
};

TEST_CASE("Member function", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    uint64_t a = 4;
    uint64_t b = 5;

    zog z{a};
    auto target_func_ptr = &zog::member_func;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>((void*)target_func_ptr), &z, b);
    REQUIRE(ret == a + b);
}

TEST_CASE("Const member function", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    uint64_t a = 4;
    uint64_t b = 5;

    zog z{a};
    auto target_func_ptr = &zog::const_member_func;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>((void*)target_func_ptr), &z, b);
    REQUIRE(ret == a + b);
}
