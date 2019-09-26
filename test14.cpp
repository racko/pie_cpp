#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter 14") {
    SECTION("Trivial") {}
    SECTION("Maybe") {}
    SECTION("head/tail") {}
    SECTION("list-ref") {}
    // list-ref: "aref"
    SECTION("Absurd") {
        define("similarly-absurd", Arrow(Absurd, Absurd), lambda([](const auto& x){ return x; }));
    }
    SECTION("Fin") {}
    // Fin: finite number. Implemented using Absurd and Maybe
    SECTION("vec-ref") {}
}
