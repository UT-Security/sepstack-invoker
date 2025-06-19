#include <stdint.h>

#include <catch2/catch_test_macros.hpp>

#include "test_common.h"
#include "sepstack_invoker.hpp"

struct uu { uint64_t a; uint64_t b; };
struct ud { uint64_t a; double b; };
struct du { double a; uint64_t b; };
struct dd { double a; double b; };

struct u64 { uint64_t val; };
struct d64 { double val; };

struct u64u64 { u64 a; u64 b; };
struct u64d64 { u64 a; d64 b; };
struct d64u64 { d64 a; u64 b; };
struct d64d64 { d64 a; d64 b; };

static uu retuu() { uu ret = {13, 14}; return ret; }
static ud retud() { ud ret = {23, 24}; return ret; }
static du retdu() { du ret = {33, 34}; return ret; }
static dd retdd() { dd ret = {43, 44}; return ret; }
static u64u64 retu64u64() { u64u64 ret = {53, 54}; return ret; }
static u64d64 retu64d64() { u64d64 ret = {63, 64}; return ret; }
static d64u64 retd64u64() { d64u64 ret = {73, 74}; return ret; }
static d64d64 retd64d64() { d64d64 ret = {83, 84}; return ret; }

#define invoke_func(func, ...) invoke_func_on_separate_stack<decltype(func)>( \
      0, UINT64_MAX, sbx_stack_end, reinterpret_cast<uintptr_t>(func), ##__VA_ARGS__);


TEST_CASE("Function with pair struct returns", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    {
        uu ret = invoke_func(retuu);
        REQUIRE(ret.a == 13);
        REQUIRE(ret.b == 14);
    }
    {
        ud ret = invoke_func(retud);
        REQUIRE(ret.a == 23);
        REQUIRE(ret.b == 24);
    }
    {
        du ret = invoke_func(retdu);
        REQUIRE(ret.a == 33);
        REQUIRE(ret.b == 34);
    }
    {
        dd ret = invoke_func(retdd);
        REQUIRE(ret.a == 43);
        REQUIRE(ret.b == 44);
    }

    {
        u64u64 ret = invoke_func(retu64u64);
        REQUIRE(ret.a.val == 53);
        REQUIRE(ret.b.val == 54);
    }
    // {
    //     u64d64 ret = invoke_func(retu64d64);
    //     REQUIRE(ret.a.val == 63);
    //     REQUIRE(ret.b.val == 64);
    // }
    // {
    //     d64u64 ret = invoke_func(retd64u64);
    //     REQUIRE(ret.a.val == 73);
    //     REQUIRE(ret.b.val == 74);
    // }
    // {
    //     d64d64 ret = invoke_func(retd64d64);
    //     REQUIRE(ret.a.val == 83);
    //     REQUIRE(ret.b.val == 84);
    // }

}

static void paruu(uu p) {
    REQUIRE(p.a == 13);
    REQUIRE(p.b == 14);
}
static void parud(ud p) {
    REQUIRE(p.a == 23);
    REQUIRE(p.b == 24);
}
static void pardu(du p) {
    REQUIRE(p.a == 33);
    REQUIRE(p.b == 34);
}
static void pardd(dd p) {
    REQUIRE(p.a == 43);
    REQUIRE(p.b == 44);
}

static void paru64u64(u64u64 p) {
    REQUIRE(p.a.val == 53);
    REQUIRE(p.b.val == 54);
}
static void paru64d64(u64d64 p) {
    REQUIRE(p.a.val == 63);
    REQUIRE(p.b.val == 64);
}
static void pard64u64(d64u64 p) {
    REQUIRE(p.a.val == 73);
    REQUIRE(p.b.val == 74);
}
static void pard64d64(d64d64 p) {
    REQUIRE(p.a.val == 83);
    REQUIRE(p.b.val == 84);
}

TEST_CASE("Function with pair struct params", "[function call]")
{
    uintptr_t sbx_stack_end;
    auto sbx_stack_alloc = get_stack_allocation(&sbx_stack_end);

    {
        uu p{13, 14};
        invoke_func(paruu, p);
    }
    {
        ud p{23, 24};
        invoke_func(parud, p);
    }
    {
        du p{33, 34};
        invoke_func(pardu, p);
    }
    {
        dd p{43, 44};
        invoke_func(pardd, p);
    }

    {
        u64u64 p{{53}, {54}};
        invoke_func(paru64u64, p);
    }
    // {
    //     u64d64 p{{63}, {64}};
    //     invoke_func(paru64d64, p);
    // }
    // {
    //     d64u64 p{{73}, {74}};
    //     invoke_func(pard64u64, p);
    // }
    // {
    //     d64d64 p{{83}, {84}};
    //     invoke_func(pard64d64, p);
    // }
}

