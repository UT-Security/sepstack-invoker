#include <iostream>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <type_traits>

#include "sepstack_invoker.hpp"

extern "C" {
__attribute__((weak)) thread_local void *callback_func = 0;

extern void *springboard_stack_change;
}


template <typename... TArgs> void printTypes() {
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
  puts(__PRETTY_FUNCTION__); // NOLINT
#elif defined(_MSC_VER)
  puts(__FUNCSIG__); // NOLINT
#else
  puts("Unsupported");
#endif
}


///////////////

class foo {
public:
  uint64_t a;
  uint64_t b;
};

class bar {
public:
  uint64_t a;
  uint64_t b;

  ~bar() { printf("Destructor called\n"); }
};

class zog {
public:
  uint64_t val;

  zog(uint64_t s) : val(s) {}

  static int static_func(int p) { return p; }

  int member_func(int p) { return val + p; }

  int const_member_func(int p) const { return val + p; }
};

struct largeStruct {
  uint64_t a1;
  uint64_t a2;
  uint64_t a3;
};

largeStruct testLargeStructReturn() {
  largeStruct ret;
  ret.a1 = 3;
  ret.a2 = 7;
  ret.a3 = 9;
  return ret;
}

extern "C" {
int testIntReturn(int a) { return a + 4; }

float testFloatReturn(float a) { return a + 4.2; }
double testDoubleReturn(double a) { return a + 4.2; }

foo testStructReturn() {
  foo ret;
  ret.a = 5;
  ret.b = 9;
  return ret;
}

uint64_t testClassParam(foo p) {
  asm volatile("");
  return p.a;
}

void testComplexClassParam(bar p) { asm volatile(""); }

enum COLOR { RED, GREEN };

COLOR testEnumReturn() { return GREEN; }

int testArrParam(int a[2]) { return a[0] + a[1]; }

int testRefParam(int &a) { return a; }

unsigned int stackParametersTestInt(unsigned int a1, unsigned int a2,
                                    unsigned int a3, unsigned int a4,
                                    unsigned int a5, unsigned int a6,
                                    unsigned int a7, unsigned int a8,
                                    unsigned int a9, unsigned int a10,
                                    unsigned int a11) {
  return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11;
}
}

int &testRefRet(int &a) { return a; }

auto app_func(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
              int a9, int a10, int a11) {
  // largeStruct ret;
  printf("In callback: %d\n", a1);
  // ret.a1 = a1;
  // ret.a2 = a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11;
  double ret = a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11;
  return ret;
}

auto testCallback() {

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

  return invoke_callback_from_separate_stack(
      0, UINT64_MAX, saved_sepstack_context->target_stack_ptr, &app_func);
}

int main(int argc, char **argv) {
  auto sbx_stack_alloc = std::make_unique<char[]>(2 * 1024 * 1024);
  uintptr_t sbx_stack_end =
      reinterpret_cast<uintptr_t>(sbx_stack_alloc.get()) + 2 * 1024 * 1024;

  sbx_stack_end -= sbx_stack_end % 32; // mod 32 = 0
  sbx_stack_end -= 8;                  // mod 32 = 8

  // auto target_func_ptr = &stackParametersTestInt;

  callback_func = (void *)&app_intercept;
  auto target_func_ptr = &testCallback;

  auto ret = invoke_func_on_separate_stack<decltype(target_func_ptr)>(
       0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(target_func_ptr));
  // , 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);

  std::cout << "Got val " << ret << "\n";

  return 0;
}