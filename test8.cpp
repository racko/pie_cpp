#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter 7.5") {
    SECTION("TODO") {}
}

TEST_CASE("Chapter 8") {
    SECTION("Eq") {
        CHECK(IsAType(Eq(Atom, quote("kale"), quote("blackberries"))));
        CHECK(IsAType(Eq(Nat, plus(one, one), two)));
        CHECK(IsAType(Eq(car(the(Pair(U, Atom), cons(Nat, quote("kale")))), nat<17>(), plus(nat<14>(), nat<3>()))));
        CHECK(IsAType(Eq(car(the(Pair(U, Atom), cons(Nat, quote("kale")))), nat<15>(), plus(nat<14>(), nat<3>()))));
        CHECK(IsA(same(nat<21>()), Eq(Nat, plus(nat<17>(), nat<4>()), plus(nat<11>(), nat<10>()))));
        CHECK(IsA(same(incr(three)), Eq(Nat, plus(two, two), four)));
    }
    // incr / add1 / plus(1)
}
