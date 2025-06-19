#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

static int testRefParam(int &a) { return a; }

static int &testRefRet(int &a) { return a; }


TEST_CASE("Function with refs", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    int a = 7;

    {
        auto target_func_ptr = &testRefParam;
        auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
        0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a);
        REQUIRE(ret == a);
    }

    // {
    //     auto target_func_ptr = &testRefRet;
    //     auto& ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
    //     0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), a);
    //     REQUIRE(&ret == &a);
    // }
}

