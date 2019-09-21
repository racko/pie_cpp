#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter 12") {
    SECTION("Even/Odd") {
        CHECK(ComputeValue(Even(ten)) == Sigma(Nat, [](const auto& half) { return Eq(Nat, ten, doub(half)); }));
        CHECK(IsA(cons(five, same(ten)), Even(ten)));
        define("thirteen-is-odd", Odd(nat<13>()), cons(six, same(nat<13>())));
    }
    // Ackermann
}
