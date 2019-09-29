#pragma once

#include "absurd.h"
#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "nat/add1.h"
#include "nat/ind_nat.h"
#include "nat/nat.h"
#include "pie_base.h"
#include "prelude/maybe.h"
#include "strings.h"

inline CONSTEXPR auto Fin = define<Fin_>(
    Arrow(Nat, U), lambda([](const auto& n) {
        return ind_Nat(n, lambda([](const auto&) { return U; }), Absurd, lambda([](const auto&, const auto& acc) {
                           return Maybe(acc);
                       }));
    }));

inline CONSTEXPR auto fzero = define<Fzero>(Pi(Nat, [](const auto& n) { return Fin(add1(n)); }),
                                            lambda([](const auto& n) { return nothing(Fin(n)); }));

inline CONSTEXPR auto fadd1 =
    define<Fadd1>(Pi(Nat, [](const auto& n) { return Arrow(Fin(n), Fin(add1(n))); }),
                  lambda([](const auto& n, const auto& smaller) { return just(Fin(n), smaller); }));
