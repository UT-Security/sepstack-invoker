#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

static float testFloatReturn(float a) { return a + 4.2; }
static double testDoubleReturn(double a) { return a + 4.2; }

// TEST_CASE("Float function", "[function call]")
// {
//     uintptr_t sbx_stack_end;
//     auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

//     float a = 3.4;

//     auto target_func_ptr = &testFloatReturn;
//     auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
//       0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a);

//     REQUIRE(ret == a+4.2);
// }

TEST_CASE("Double function", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    double a = 3.4;

    auto target_func_ptr = &testDoubleReturn;
    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a);

    REQUIRE(ret == a+4.2);
}
