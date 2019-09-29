#pragma once

#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include "strings.h"

namespace church {
inline CONSTEXPR auto Nat = define<Nat_>(U, Pi(U, [](const auto& X) { return Arrow(X, Arrow(X, X), X); }));
inline CONSTEXPR auto zero = define<Zero>(Nat, lambda([](const auto&, const auto& b, const auto&) { return b; }));
inline CONSTEXPR auto add1 =
    define<Add1>(Arrow(Nat, Nat), lambda([](const auto& smaller, const auto& X, const auto& b, const auto& step) {
                     return step(smaller(X, b, step));
                 }));
} // namespace church
