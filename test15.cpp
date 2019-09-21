#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter 15") {
    SECTION("not") {}
    SECTION("front") {}
    // Vec version of head
    SECTION("principle of excluded middle") {}
    // last page: Definition of Dec
}
