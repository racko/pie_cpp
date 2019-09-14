#pragma once

#include "pie.h"

constexpr auto one{define("one", Nat, add1(zero))};
constexpr auto two{define("two", Nat, add1(one))};
constexpr auto three{define("three", Nat, add1(two))};
constexpr auto four{define("four", Nat, add1(three))};
constexpr auto five{define("five", Nat, add1(four))};
constexpr auto six{define("six", Nat, add1(five))};
constexpr auto seven{define("seven", Nat, add1(six))};
constexpr auto eight{define("eight", Nat, add1(seven))};
constexpr auto nine{define("nine", Nat, add1(eight))};
constexpr auto ten{define("ten", Nat, add1(nine))};

// NOTE: NEVER STORE PIE<T> IN A LAMBDA. OR ANYWHERE FOR WHAT IT'S WORTH. SLICING!
// USE [x = x.derived] OR SIMILAR.

constexpr auto iter_Nat = define("iter-Nat",
                                 Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(X, X), X); }),
                                 lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                     return ind_Nat(target,
                                                    lambda([X](const auto&) { return X; }),
                                                    base,
                                                    lambda([step](const auto&, const auto& acc) { return step(acc); }));
                                 }));

constexpr auto step_rec_Nat =
    define("step-rec-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X, X), Pair(Nat, X), Pair(Nat, X)); }),
           lambda([](const auto&, const auto& step, const auto& acc) {
               return cons(add1(car(acc)), step(car(acc), cdr(acc)));
           }));

constexpr auto rec_Nat = define("rec-Nat",
                                Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
                                lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                    return cdr(iter_Nat(Pair(Nat, X), target, cons(zero, base), step_rec_Nat(X, step)));
                                }));

constexpr auto step_which_Nat =
    define("step-which-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
           lambda([](const auto&, const auto& step, const auto& acc, const auto&) { return step(acc); }));

constexpr auto which_Nat = define("which-Nat",
                                  Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
                                  lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                      return rec_Nat(X, target, base, step_which_Nat(X, step));
                                  }));

constexpr auto step_plus = define("step-+", Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

constexpr auto plus = define(
    "+", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) { return iter_Nat(Nat, lhs, rhs, step_plus); }));

constexpr auto zerop = define("zerop", Arrow(Nat, Atom), lambda([](const auto& x) {
                                  return iter_Nat(Atom, x, quote("t"), lambda([](const auto&) { return quote("nil"); }));
                              }));

constexpr auto step_gauss = define("step-gauss", Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) {
                                       return plus(add1(smaller), acc);
                                   }));

constexpr auto gauss =
    define("gauss", Arrow(Nat, Nat), lambda([](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));

constexpr auto times = define(
    "*", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) { return iter_Nat(Nat, lhs, zero, plus(rhs)); }));

constexpr auto flip =
    define("flip",
           Pi(U, [](const auto& A) { return Pi(U, [A](const auto& D) { return Arrow(Pair(A, D), Pair(D, A)); }); }),
           lambda([](const auto&, const auto&, const auto& p) { return cons(cdr(p), car(p)); }));

constexpr auto incr =
    define("incr", Arrow(Nat, Nat), lambda([](const auto& n) { return iter_Nat(Nat, n, one, plus(one)); }));

constexpr auto base_incr_equals_add1 = define("base-incr=add1", Eq(Nat, incr(zero), add1(zero)), same(add1(zero)));

constexpr auto mot_incr_equals_add1 =
    define("mot-incr=add1", Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, incr(k), add1(k)); }));

constexpr auto step_incr_equals_add1 = define(
    "step-incr=add1",
    Pi(Nat,
       [](const auto& smaller) {
           return Arrow(Eq(Nat, incr(smaller), add1(smaller)), Eq(Nat, add1(incr(smaller)), add1(add1(smaller))));
       }),
    lambda([](const auto&, const auto& almost) { return cong(almost, plus(one)); }));

// constexpr auto incr_equals_add1 = define("incr=add1", Pi(), );
// constexpr auto plus1_equals_add1 = define("+1=add1", Pi(), );
