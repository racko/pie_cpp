#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct Flip_pair {
    static inline constexpr const char* value = "flip-pair";
};

TEST_CASE("Chapter 4") {
    SECTION("Pi") {
        constexpr auto flip_pair = define<Flip_pair>(
            Pi(U, [](const auto& A) { return Pi(U, [A](const auto& D) { return Arrow(Pair(A, D), Pair(D, A)); }); }),
            lambda([](const auto&, const auto&, const auto& p) { return cons(cdr(p), car(p)); }));

        CHECK(ComputeValue(flip_pair(Nat, Atom)) == lambda([](const auto& p) { return cons(cdr(p), car(p)); }));
    }
}
