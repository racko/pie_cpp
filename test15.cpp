#include "pie.h"
#include "prelude/prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct Powdered {
    static inline constexpr const char* value = "powdered";
};

struct Glazed {
    static inline constexpr const char* value = "glazed";
};

struct Donut_absurdity {
    static inline constexpr const char* value = "donut-absurdity";
};

struct One_not_six {
    static inline constexpr const char* value = "one-not-six";
};

struct Pem_not_false {
    static inline constexpr const char* value = "pem-not-false";
};

TEST_CASE("Chapter 15") {
    SECTION("not") {
        define<Donut_absurdity>(Arrow(Eq(Nat, zero, six), Eq(Atom, quote<Powdered>(), quote<Glazed>())),
                                lambda([](const auto& zero_equals_six) {
                                    return ind_Absurd(zero_not_add1(five, zero_equals_six),
                                                      Eq(Atom, quote<Powdered>(), quote<Glazed>()));
                                }));
        define<One_not_six>(Arrow(Eq(Nat, one, six), Absurd), lambda([](const auto& one_equals_six) {
                                return zero_not_add1(four, sub1_eq(zero, five, one_equals_six));
                            }));
    }
    SECTION("front") {}
    // Vec version of head
    SECTION("principle of excluded middle") {
        define<Pem_not_false>(
            Pi(U, [](const auto& P) { return Arrow(Arrow(Either(P, Arrow(P, Absurd)), Absurd), Absurd); }),
            lambda([](const auto&, const auto& pem_false) {
                return pem_false(right(lambda([pem_false](const auto& p) { return pem_false(left(p)); })));
            }));
    }
    // last page: Definition of Dec
}
