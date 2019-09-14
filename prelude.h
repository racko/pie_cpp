#pragma once

#include "pie.h"

constexpr const auto one{define("one", Nat, add1(zero))};
constexpr const auto two{define("two", Nat, add1(one))};
constexpr const auto three{define("three", Nat, add1(two))};
constexpr const auto four{define("four", Nat, add1(three))};
constexpr const auto five{define("five", Nat, add1(four))};
constexpr const auto six{define("six", Nat, add1(five))};
constexpr const auto seven{define("seven", Nat, add1(six))};
constexpr const auto eight{define("eight", Nat, add1(seven))};
constexpr const auto nine{define("nine", Nat, add1(eight))};
constexpr const auto ten{define("ten", Nat, add1(nine))};

// NOTE: NEVER STORE PIE<T> IN A LAMBDA. OR ANYWHERE FOR WHAT IT'S WORTH. SLICING!
// USE [x = x.derived] OR SIMILAR.

constexpr const auto step_rec_Nat =
    define("step-rec-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X, X), Pair(Nat, X), Pair(Nat, X)); }),
           lambda([](const auto&, const auto& step, const auto& acc) {
               return cons(add1(car(acc)), step(car(acc), cdr(acc)));
           }));

constexpr const auto rec_Nat = define("rec-Nat",
                            Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
                            lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                return cdr(iter_Nat(target, cons(zero, base), step_rec_Nat(X, step)));
                            }));

constexpr const auto step_which_Nat =
    define("step-which-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
           lambda([](const auto&, const auto& step, const auto& acc, const auto&) { return step(acc); }));

constexpr const auto which_Nat = define("which-Nat",
                              Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
                              lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                  return rec_Nat(X, target, base, step_which_Nat(X, step));
                              }));

constexpr const auto incr = define("incr", Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

constexpr const auto plus = define(
    "+", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) { return iter_Nat(lhs, rhs, incr); }));

constexpr const auto zerop = define("zerop", Arrow(Nat, Atom), lambda([](const auto& x) {
                              return iter_Nat(x, quote("t"), lambda([](const auto&) { return quote("nil"); }));
                          }));

constexpr const auto step_gauss = define("step-gauss", Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) {
                                   return plus(add1(smaller), acc);
                               }));

constexpr const auto gauss =
    define("gauss", Arrow(Nat, Nat), lambda([](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));

constexpr const auto times = define("*", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) {
                              return iter_Nat(lhs, zero, plus(rhs));
                          }));

constexpr const auto flip =
    define("flip",
           Pi(U, [](const auto& A) { return Pi(U, [A](const auto& D) { return Arrow(Pair(A, D), Pair(D, A)); }); }),
           lambda([](const auto&, const auto&, const auto& p) { return cons(cdr(p), car(p)); }));
