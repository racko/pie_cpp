#pragma once

#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include "strings.h"

inline CONSTEXPR auto constant =
    define<Const>(Pi(U, [](const auto& A) { return Pi(U, [A](const auto& B) { return Arrow(A, B, A); }); }),
                  lambda([](const auto&, const auto&, const auto& a, const auto&) { return a; }));

inline CONSTEXPR auto flip = define<Flip>(
    Pi(U,
       [](const auto& A) {
           return Pi(U, [A](const auto& B) {
               return Pi(U, [A, B](const auto& C) { return Arrow(Arrow(A, B, C), B, A, C); });
           });
       }),
    lambda([](const auto&, const auto&, const auto&, const auto& f, const auto& b, const auto& a) { return f(a, b); }));
