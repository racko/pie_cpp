#include "prelude.h"

#include <catch2/catch.hpp>
#include <codecvt>
#include <iostream>
#include <locale>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

// std::wbuffer_convert<std::codecvt_utf8<wchar_t>> cout_conv(std::cout.rdbuf());
// std::wbuffer_convert<std::codecvt_utf8<wchar_t>> cerr_conv(std::cerr.rdbuf());

TEST_CASE("Chapter 2.5") {
    SECTION("the") {}
}

TEST_CASE("Chapter 3") {
    SECTION("iter-Nat") {
        CHECK(Normalize(iter_Nat(five, three, lambda([](auto smaller) { return add1(smaller); }))) == eight);
    }
    SECTION("rec-Nat") {
        CHECK(ComputeValue(zerop(nat<37>())) == quote(L"nil"));
        CHECK(Normalize(gauss(zero)) == zero);
        CHECK(Normalize(gauss(one)) == one);
        CHECK(Normalize(gauss(two)) == three);
        CHECK(Normalize(times(nat<2>(), nat<29>())) == nat<58>());
    }
}

TEST_CASE("Chapter 4") {
    SECTION("Pi") {
        CHECK(ComputeValue(flip(Nat, Atom)) == lambda([](const auto& p) { return cons(cdr(p), car(p)); }));
    }
}

TEST_CASE("Chapter 5") {
    SECTION("List") {}
}

TEST_CASE("Chapter 6") {
    SECTION("Vec") {}
}

TEST_CASE("Chapter 7") {
    SECTION("ind-Nat") {}
}

TEST_CASE("Chapter 7.5") {
    SECTION("TODO") {}
}

TEST_CASE("Chapter 8") {
    SECTION("Eq") {}
}
