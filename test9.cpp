#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter 9") {
    SECTION("replace") {}
    const auto add1_plus_equals_plus_add1 = [] {
        const auto mot = define("mot-add1+=+add1", Arrow(Nat, Nat, U), lambda([](const auto& j, const auto& k) {
                                    return Eq(Nat, add1(plus(k, j)), plus(k, add1(j)));
                                }));
        const auto step =
            define("step-add1+=+add1",
                   Pi(Nat,
                      [mot](const auto& j) {
                          return Pi(Nat, [mot, j](const auto& smaller) {
                              return Arrow(mot(j, smaller), mot(j, add1(smaller)));
                          });
                      }),
                   lambda([](const auto& j, const auto& smaller, const auto& smaller_proof) {
                       return cong(Nat, Nat, add1(plus(smaller, j)), plus(smaller, add1(j)), smaller_proof, plus(one));
                   }));
        return define(
            "add1+=+add1",
            Pi(Nat,
               [](const auto& n) {
                   return Pi(Nat, [n](const auto& j) { return Eq(Nat, add1(plus(n, j)), plus(n, add1(j))); });
               }),
            lambda([mot, step](const auto& n, const auto& j) { return ind_Nat(n, mot(j), same(add1(j)), step(j)); }));
    }();

    [[maybe_unused]] const auto twice_equals_double = [add1_plus_equals_plus_add1] {
        const auto mot =
            define("mot-twice=double", Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, twice(k), doub(k)); }));

        const auto step = [mot, add1_plus_equals_plus_add1] {
            const auto mot_step =
                define("mot-step-twice=double", Arrow(Nat, Nat, U), lambda([](const auto& smaller, const auto& k) {
                           return Eq(Nat, add1(k), add1(add1(doub(smaller))));
                       }));

            return define(
                "step-twice=double",
                Pi(Nat, [mot](const auto& smaller) { return Arrow(mot(smaller), mot(add1(smaller))); }),
                lambda([add1_plus_equals_plus_add1, mot_step](const auto& smaller, const auto& smaller_proof) {
                    return replace(Nat,
                                   add1(plus(smaller, smaller)),
                                   plus(smaller, add1(smaller)),
                                   add1_plus_equals_plus_add1(smaller, smaller),
                                   mot_step(smaller),
                                   cong(Nat, Nat, twice(smaller), doub(smaller), smaller_proof, plus(two)));
                }));
        }();

        return define("twice=double",
                      Pi(Nat, [](const auto& n) { return Eq(Nat, twice(n), doub(n)); }),
                      lambda([mot, step](const auto& n) { return ind_Nat(n, mot, same(zero), step); }));
    }();

    // twiceVec / doubleVec
    SECTION("symm") {}
}
