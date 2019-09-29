#include "pie.h"
#include "prelude/prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct Step_add1_plus_equals_plus_add1 {
    static inline constexpr const char* value = "step-add1+=+add1";
};

struct Mot_add1_plus_equals_plus_add1 {
    static inline constexpr const char* value = "mot-add1+=+add1";
};

struct Add1_plus_equals_plus_add1 {
    static inline constexpr const char* value = "add1+=+add1";
};

struct Mot_twice_equals_double {
    static inline constexpr const char* value = "mot-twice=double";
};

struct Mot_step_twice_equals_double {
    static inline constexpr const char* value = "mot-step-twice=double";
};

struct Step_twice_equals_double {
    static inline constexpr const char* value = "step-twice=double";
};

struct Twice_equals_double {
    static inline constexpr const char* value = "twice=double";
};

TEST_CASE("Chapter 9") {
    SECTION("replace") {}
    const auto add1_plus_equals_plus_add1 = [] {
        const auto mot = define<Mot_add1_plus_equals_plus_add1>(
            Arrow(Nat, Nat, U),
            lambda([](const auto& j, const auto& k) { return Eq(Nat, add1(plus(k, j)), plus(k, add1(j))); }));
        const auto step = define<Step_add1_plus_equals_plus_add1>(
            Pi(Nat,
               [mot](const auto& j) {
                   return Pi(Nat,
                             [mot, j](const auto& smaller) { return Arrow(mot(j, smaller), mot(j, add1(smaller))); });
               }),
            lambda([](const auto& j, const auto& smaller, const auto& smaller_proof) {
                return cong(Nat, Nat, add1(plus(smaller, j)), plus(smaller, add1(j)), smaller_proof, plus(one));
            }));
        return define<Add1_plus_equals_plus_add1>(
            Pi(Nat,
               [](const auto& n) {
                   return Pi(Nat, [n](const auto& j) { return Eq(Nat, add1(plus(n, j)), plus(n, add1(j))); });
               }),
            lambda([mot, step](const auto& n, const auto& j) { return ind_Nat(n, mot(j), same(add1(j)), step(j)); }));
    }();

    [[maybe_unused]] const auto twice_equals_double = [add1_plus_equals_plus_add1] {
        const auto mot = define<Mot_twice_equals_double>(
            Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, twice(k), doub(k)); }));

        const auto step = [mot, add1_plus_equals_plus_add1] {
            const auto mot_step = define<Mot_step_twice_equals_double>(
                Arrow(Nat, Nat, U),
                lambda([](const auto& smaller, const auto& k) { return Eq(Nat, add1(k), add1(add1(doub(smaller)))); }));

            return define<Step_twice_equals_double>(
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

        return define<Twice_equals_double>(
            Pi(Nat, [](const auto& n) { return Eq(Nat, twice(n), doub(n)); }),
            lambda([mot, step](const auto& n) { return ind_Nat(n, mot, same(zero), step); }));
    }();

    // twiceVec / doubleVec
    SECTION("symm") {}
}
