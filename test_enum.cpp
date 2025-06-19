#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

enum COLOR { RED, GREEN };

static void testEnumParam(COLOR c) { REQUIRE(c == GREEN); }
static COLOR testEnumReturn() { return GREEN; }

TEST_CASE("Enum function", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    COLOR a = GREEN;

    {
        auto target_func_ptr = &testEnumParam;
        invoke_func_on_separate_stack<decltype(target_func_ptr)>(
        0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a);
    }

    {
        auto target_func_ptr = &testEnumReturn;
        auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
        0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr));

        REQUIRE(ret == a);
    }
}
