#pragma once

#include "absurd.h"
#include "atom.h"
#include "either/either.h"
#include "either/ind_either.h"
#include "either/left.h"
#include "either/right.h"
#include "eq/eq.h"
#include "eq/same.h"
#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "nat/add1.h"
#include "nat/ind_nat.h"
#include "nat/nat.h"
#include "nat/zero.h"
#include "pair/car.h"
#include "pair/cdr.h"
#include "pair/cons.h"
#include "pair/sigma.h"
#include "pie_base.h"
#include "prelude/dec.h"
#include "prelude/equality.h"
#include "prelude/function.h"
#include "prelude/nats.h"
#include "strings.h"
#include "trivial.h"

inline CONSTEXPR auto const_ind_Mot =
    define<Const_ind_Mot>(Arrow(U, Nat, U), lambda([](const auto& X, const auto&) { return X; }));

inline CONSTEXPR auto iter_Nat =
    define<Iter_Nat>(Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(X, X), X); }),
                     lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                         return ind_Nat(target, const_ind_Mot(X), base, lambda([step](const auto&, const auto& acc) {
                                            return step(acc);
                                        }));
                     }));

inline CONSTEXPR auto rec_Nat =
    define<Rec_Nat>(Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
                    lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                        return ind_Nat(target, const_ind_Mot(X), base, step);
                    }));

inline CONSTEXPR auto which_Nat = [] {
    CONSTEXPR auto step_which_Nat = define<Step_which_Nat>(
        Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
        lambda([](const auto&, const auto& step, const auto& n_minus_1, const auto&) { return step(n_minus_1); }));

    return define<Which_Nat>(
        Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
        lambda([step_which_Nat](const auto& X, const auto& target, const auto& base, const auto& step) {
            return rec_Nat(X, target, base, step_which_Nat(X, step));
        }));
}();

inline CONSTEXPR auto plus = [] {
    CONSTEXPR auto step_plus = define<Step_plus>(Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

    return define<Plus>(Arrow(Nat, Nat, Nat), lambda([step_plus](const auto& lhs, const auto& rhs) {
                            return iter_Nat(Nat, lhs, rhs, step_plus);
                        }));
}();

inline CONSTEXPR auto zerop = define<Zerop>(
    Arrow(Nat, Atom),
    lambda([](const auto& x) { return iter_Nat(Atom, x, quote<Nil>(), constant(Atom, Atom, quote<Nil>())); }));

inline CONSTEXPR auto gauss = [] {
    CONSTEXPR auto step_gauss = define<Step_gauss>(
        Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) { return plus(add1(smaller), acc); }));

    return define<Gauss>(Arrow(Nat, Nat),
                         lambda([step_gauss](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));
}();

inline CONSTEXPR auto times = define<Times>(
    Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) { return iter_Nat(Nat, lhs, zero, plus(rhs)); }));

inline CONSTEXPR auto incr =
    define<Incr>(Arrow(Nat, Nat), lambda([](const auto& n) { return iter_Nat(Nat, n, one, plus(one)); }));

inline CONSTEXPR auto plus1_equals_add1 =
    define<Plus1_equals_add1>(Pi(Nat, [](const auto& n) { return Eq(Nat, plus(one, n), add1(n)); }),
                              lambda([](const auto& n) { return same(add1(n)); }));

inline CONSTEXPR auto incr_equals_add1 = [] {
    CONSTEXPR auto base_incr_equals_add1 =
        define<Base_incr_equals_add1>(Eq(Nat, incr(zero), add1(zero)), same(add1(zero)));

    CONSTEXPR auto mot_incr_equals_add1 =
        define<Mot_incr_equals_add1>(Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, incr(k), add1(k)); }));

    CONSTEXPR auto step_incr_equals_add1 = define<Step_incr_equals_add1>(
        Pi(Nat,
           [](const auto& smaller) {
               return Arrow(Eq(Nat, incr(smaller), add1(smaller)), Eq(Nat, add1(incr(smaller)), add1(add1(smaller))));
           }),
        lambda([](const auto& smaller, const auto& almost) {
            return cong(Nat, Nat, incr(smaller), add1(smaller), almost, plus(one));
        }));

    return define<Incr_equals_add1>(
        Pi(Nat, [](const auto& n) { return Eq(Nat, incr(n), add1(n)); }),
        lambda([mot_incr_equals_add1, base_incr_equals_add1, step_incr_equals_add1](const auto& n) {
            return ind_Nat(n, mot_incr_equals_add1, base_incr_equals_add1, step_incr_equals_add1);
        }));
}();

// CONSTEXPR auto doub = define("double", Arrow(Nat, Nat), lambda([](const auto& n) { return times(n, two); }));
inline CONSTEXPR auto doub = define<Doub>(Arrow(Nat, Nat), flip(Nat, Nat, Nat, times, two));

inline CONSTEXPR auto twice = define<Twice>(Arrow(Nat, Nat), lambda([](const auto& n) { return plus(n, n); }));

inline CONSTEXPR auto Even = define<Even_>(
    Arrow(Nat, U),
    lambda([](const auto& n) { return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, doub(half)); }); }));

inline CONSTEXPR auto Odd = define<Odd_>(
    Arrow(Nat, U),
    lambda([](const auto& n) { return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, add1(doub(half))); }); }));

inline CONSTEXPR auto zero_is_even = define<Zero_is_even>(Even(zero), cons(zero, same(zero)));

inline CONSTEXPR auto plus_two_even = define<Plus_two_even>(
    Pi(Nat, [](const auto& n) { return Arrow(Even(n), Even(plus(two, n))); }),
    lambda([](const auto& n, const auto& n_even) {
        return cons(add1(car(n_even)), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(two)));
    }));

inline CONSTEXPR auto two_is_even = define<Two_is_even>(Even(two), plus_two_even(zero, zero_is_even));

inline CONSTEXPR auto one_is_odd = define<One_is_odd>(Odd(one), cons(zero, same(one)));

inline CONSTEXPR auto even_implies_add1_odd = define<Even_implies_add1_odd>(
    Pi(Nat, [](const auto& n) { return Arrow(Even(n), Odd(add1(n))); }), lambda([](const auto& n, const auto& n_even) {
        return cons(car(n_even), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(one)));
    }));

inline CONSTEXPR auto odd_implies_add1_even = define<Odd_implies_add1_even>(
    Pi(Nat, [](const auto& n) { return Arrow(Odd(n), Even(add1(n))); }), lambda([](const auto& n, const auto& n_odd) {
        return cons(add1(car(n_odd)), cong(Nat, Nat, n, add1(doub(car(n_odd))), cdr(n_odd), plus(one)));
    }));

inline CONSTEXPR auto even_or_odd = [] {
    CONSTEXPR auto mot_even_or_odd =
        define<Mot_even_or_odd>(Arrow(Nat, U), lambda([](const auto& k) { return Either(Even(k), Odd(k)); }));

    CONSTEXPR auto base_even_or_odd = define<Base_even_or_odd>(mot_even_or_odd(zero), left(zero_is_even));

    CONSTEXPR auto step_even_or_odd = [mot_even_or_odd] {
        CONSTEXPR auto mot_step_even_or_odd = define<Mot_step_even_or_odd>(
            Pi(Nat, [](const auto& n) { return Arrow(Either(Even(n), Odd(n)), U); }),
            lambda([](const auto& n, const auto& /*n_even_or_odd*/) { return Either(Even(add1(n)), Odd(add1(n))); }));

        CONSTEXPR auto base_left_step_even_or_odd = define<Base_left_step_even_or_odd>(
            Pi(Nat,
               [mot_step_even_or_odd](const auto& n) {
                   return Pi(Even(n), [mot_step_even_or_odd, n](const auto& n_even) {
                       return mot_step_even_or_odd(n, left(n_even));
                   });
               }),
            lambda([](const auto& n, const auto& n_even) { return right(even_implies_add1_odd(n, n_even)); }));

        CONSTEXPR auto base_right_step_even_or_odd = define<Base_right_step_even_or_odd>(
            Pi(Nat,
               [mot_step_even_or_odd](const auto& n) {
                   return Pi(Odd(n), [mot_step_even_or_odd, n](const auto& n_odd) {
                       return mot_step_even_or_odd(n, right(n_odd));
                   });
               }),
            lambda([](const auto& n, const auto& n_odd) { return left(odd_implies_add1_even(n, n_odd)); }));

        return define<Step_even_or_odd>(
            Pi(Nat, [mot_even_or_odd](const auto& n) { return Arrow(mot_even_or_odd(n), mot_even_or_odd(add1(n))); }),
            lambda([mot_step_even_or_odd, base_left_step_even_or_odd, base_right_step_even_or_odd](
                       const auto& n, const auto& n_even_or_odd) {
                return ind_Either(n_even_or_odd,
                                  mot_step_even_or_odd(n),
                                  base_left_step_even_or_odd(n),
                                  base_right_step_even_or_odd(n));
            }));
    }();

    return define<Even_or_odd>(Pi(Nat, [](const auto& n) { return Either(Even(n), Odd(n)); }),
                               lambda([mot_even_or_odd, base_even_or_odd, step_even_or_odd](const auto& n) {
                                   return ind_Nat(n, mot_even_or_odd, base_even_or_odd, step_even_or_odd);
                               }));
}();

inline CONSTEXPR auto use_Nat_eq = [] {
    CONSTEXPR auto eq_consequence = define<Eq_consequence>(
        Arrow(Nat, Nat, U), lambda([](const auto& n, const auto& j) {
            return ind_Nat(
                n,
                lambda([](const auto&) { return U; }),
                ind_Nat(j, lambda([](const auto&) { return U; }), Trivial, lambda([](const auto&, const auto&) {
                            return Absurd;
                        })),
                lambda([j](const auto& n_minus_1, const auto&) {
                    return ind_Nat(j,
                                   lambda([](const auto&) { return U; }),
                                   Absurd,
                                   lambda([n_minus_1](const auto& j_minus_1, const auto&) {
                                       return Eq(Nat, n_minus_1, j_minus_1);
                                   }));
                }));
        }));

    CONSTEXPR auto eq_consequence_same = define<Eq_consequence_same>(
        Pi(Nat, [eq_consequence](const auto& n) { return eq_consequence(n, n); }),
        lambda([eq_consequence](const auto& n) {
            return ind_Nat(n,
                           lambda([eq_consequence](const auto& k) { return eq_consequence(k, k); }),
                           sole,
                           lambda([](const auto& n_minus_1, const auto&) { return same(n_minus_1); }));
        }));

    return define<Use_Nat_eq>(
        Pi(Nat,
           [eq_consequence](const auto& n) {
               return Pi(Nat,
                         [eq_consequence, n](const auto& j) { return Arrow(Eq(Nat, n, j), eq_consequence(n, j)); });
           }),
        lambda([eq_consequence, eq_consequence_same](const auto& n, const auto& j, const auto& n_equals_j) {
            return replace(Nat,
                           n,
                           j,
                           n_equals_j,
                           lambda([eq_consequence, n](const auto& k) { return eq_consequence(n, k); }),
                           eq_consequence_same(n));
        }));
}();

inline CONSTEXPR auto zero_not_add1 =
    define<Zero_not_add1>(Pi(Nat, [](const auto& n) { return Arrow(Eq(Nat, zero, add1(n)), Absurd); }),
                          lambda([](const auto& n) { return use_Nat_eq(zero, add1(n)); }));
inline CONSTEXPR auto sub1_eq = define<Sub1_eq>(
    Pi(Nat,
       [](const auto& n) {
           return Pi(Nat, [n](const auto& j) { return Arrow(Eq(Nat, add1(n), add1(j)), Eq(Nat, n, j)); });
       }),
    lambda([](const auto& n, const auto& j) { return use_Nat_eq(add1(n), add1(j)); }));

inline CONSTEXPR auto is_zero = define<Is_zero>(
    Pi(Nat, [](const auto& n) { return Dec(Eq(Nat, zero, n)); }), lambda([](const auto& n) {
        return ind_Nat(n,
                       lambda([](const auto& k) { return Dec(Eq(Nat, zero, k)); }),
                       left(same(zero)),
                       lambda([](const auto& n_minus_1, const auto&) { return right(zero_not_add1(n_minus_1)); }));
    }));
