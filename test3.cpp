#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page


TEST_CASE("Chapter 2.5") {
    SECTION("the") {}
}

TEST_CASE("Chapter 3") {
    SECTION("iter-Nat") {
        CHECK(Normalize(iter_Nat(Nat, five, three, lambda([](auto smaller) { return add1(smaller); }))) == eight);
    }
    SECTION("rec-Nat") {
        CHECK(ComputeValue(zerop(nat<37>())) == quote("nil"));
        CHECK(Normalize(gauss(zero)) == zero);
        CHECK(Normalize(gauss(one)) == one);
        CHECK(Normalize(gauss(two)) == three);
        CHECK(Normalize(times(nat<2>(), nat<29>())) == nat<58>());
    }
}
