#include "pie.h"
#include "prelude/prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct Are_nats_equal {
    static inline constexpr const char* value = "nat=?";
};

TEST_CASE("Chapter 16") {
    SECTION("Dec") {}
    SECTION("nat=-Dec") {
        define<Are_nats_equal>(
            Pi(Nat, [](const auto& n) { return Pi(Nat, [n](const auto& j) { return Dec(Eq(Nat, n, j)); }); }),
            lambda([](const auto& n, const auto& j) {
                return ind_Nat(
                    n,
                    lambda([](const auto& k) { return Pi(Nat, [k](const auto& i) { return Dec(Eq(Nat, k, i)); }); }),
                    is_zero,
                    lambda([](const auto& n_minus_1, const auto& is_equal_to_n_minus_1) {
                        return lambda([n_minus_1,
                                       is_equal_to_n_minus_1](const auto& i) /* -> "is_equal_to_add1_n_minus_1" */ {
                            return ind_Nat(
                                i,
                                lambda([n_minus_1](const auto& k) { return Dec(Eq(Nat, add1(n_minus_1), k)); }),
                                right(use_Nat_eq(add1(n_minus_1), zero)),
                                lambda([n_minus_1, is_equal_to_n_minus_1](const auto& i_minus_1, const auto&) {
                                    return ind_Either(
                                        is_equal_to_n_minus_1(i_minus_1),
                                        lambda([n_minus_1, i_minus_1](const auto&) {
                                            return Dec(Eq(Nat, add1(n_minus_1), add1(i_minus_1)));
                                        }),
                                        lambda([n_minus_1, i_minus_1](const auto& n_minus_1_equals_i_minus_1) {
                                            return left(cong(
                                                Nat, Nat, n_minus_1, i_minus_1, n_minus_1_equals_i_minus_1, plus(one)));
                                        }),
                                        lambda([n_minus_1, i_minus_1](const auto& n_minus_1_does_not_equal_i_minus_1) {
                                            return right(
                                                lambda([n_minus_1, i_minus_1, n_minus_1_does_not_equal_i_minus_1](
                                                           const auto& n_equals_i) {
                                                    return n_minus_1_does_not_equal_i_minus_1(
                                                        sub1_eq(n_minus_1, i_minus_1, n_equals_i));
                                                }));
                                        }));
                                }));
                        });
                    }))(j);
            }));
    }
}
