#include <stdint.h>

#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

class bar {
public:
  uint64_t a;
  uint64_t b;

  ~bar() { printf("Destructor called\n"); }
};

static void testComplexClassParam(bar p) {
    REQUIRE(p.a == 4);
    REQUIRE(p.b == 5);
}

TEST_CASE("Function with complex class param", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    bar p{4, 5};
    auto target_func_ptr = &testComplexClassParam;
    invoke_func_on_separate_stack<decltype(target_func_ptr)>(
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr), p);
}
