#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

static double app_func(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
              int a9, int a10, int a11) {
  printf("In callback: %d\n", a1);
  double ret = a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11;
  return ret;
}

static auto testCallback() {
  uintptr_t springboard_addr =
      reinterpret_cast<uintptr_t>(&springboard_stack_change);

  using func_ptr_t = decltype(&app_func);
  func_ptr_t target_func_ptr = 0;
  memcpy(reinterpret_cast<void *>(&target_func_ptr), &springboard_addr,
         sizeof(void *));
  auto ret = (*target_func_ptr)(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  return ret;
}


void app_intercept() {
  printf("In callback intercept\n");
  return invoke_callback_from_separate_stack(0, UINT64_MAX, &app_func);
}


TEST_CASE("Callback test", "[callback]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    callback_func = (void *)&app_intercept;
    auto target_func_ptr = &testCallback;

    auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
        0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr));

    double expected = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11;
    REQUIRE(ret == expected);
}
