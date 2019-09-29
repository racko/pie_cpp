#pragma once

#include "either/either.h"
#include "either/left.h"
#include "either/right.h"
#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include "strings.h"
#include "trivial.h"

inline CONSTEXPR auto Maybe = define<Maybe_>(Arrow(U, U), lambda([](const auto& X) { return Either(X, Trivial); }));

inline CONSTEXPR auto nothing =
    define<Nothing>(Pi(U, [](const auto& E) { return Maybe(E); }), lambda([](const auto&) { return right(sole); }));

inline CONSTEXPR auto just = define<Just>(Pi(U, [](const auto& E) { return Arrow(E, Maybe(E)); }),
                                          lambda([](const auto&, const auto& x) { return left(x); }));
