#pragma once

#include "pie.h"

inline auto one{define(L"one", Nat, add1(zero))};
inline auto two{define(L"two", Nat, add1(one))};
inline auto three{define(L"three", Nat, add1(two))};
inline auto four{define(L"four", Nat, add1(three))};
inline auto five{define(L"five", Nat, add1(four))};
inline auto six{define(L"six", Nat, add1(five))};
inline auto seven{define(L"seven", Nat, add1(six))};
inline auto eight{define(L"eight", Nat, add1(seven))};
inline auto nine{define(L"nine", Nat, add1(eight))};
inline auto ten{define(L"ten", Nat, add1(nine))};

// NOTE: NEVER STORE PIE<T> IN A LAMBDA. OR ANYWHERE FOR WHAT IT'S WORTH. SLICING!
// USE [x = x.derived] OR SIMILAR.

const auto step_rec_Nat =
    define(L"step-rec-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X, X), Pair(Nat, X), Pair(Nat, X)); }),
           lambda([](const auto&, const auto& step, const auto& acc) {
               return cons(add1(car(acc)), step(car(acc), cdr(acc)));
           }));

const auto rec_Nat = define(L"rec-Nat",
                            Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
                            lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                return cdr(iter_Nat(target, cons(zero, base), step_rec_Nat(X, step)));
                            }));

const auto step_which_Nat =
    define(L"step-which-Nat",
           Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
           lambda([](const auto&, const auto& step, const auto& acc, const auto&) { return step(acc); }));

const auto which_Nat = define(L"which-Nat",
                              Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
                              lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                                  return rec_Nat(X, target, base, step_which_Nat(X, step));
                              }));

const auto incr = define(L"incr", Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

const auto plus = define(
    L"+", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) { return iter_Nat(lhs, rhs, incr); }));

const auto zerop = define(L"zerop", Arrow(Nat, Atom), lambda([](const auto& x) {
                              return iter_Nat(x, quote(L"t"), lambda([](const auto&) { return quote(L"nil"); }));
                          }));

const auto step_gauss = define(L"step-gauss", Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) {
                                   return plus(add1(smaller), acc);
                               }));

const auto gauss =
    define(L"gauss", Arrow(Nat, Nat), lambda([](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));

const auto times = define(L"*", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) {
                              return iter_Nat(lhs, zero, plus(rhs));
                          }));

const auto flip =
    define(L"flip",
           Pi(U, [](const auto& A) { return Pi(U, [A](const auto& D) { return Arrow(Pair(A, D), Pair(D, A)); }); }),
           lambda([](const auto&, const auto&, const auto& p) { return cons(cdr(p), car(p)); }));
