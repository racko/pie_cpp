#pragma once

#include "absurd.h"
#include "either/either.h"
#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include "strings.h"

inline CONSTEXPR auto Dec =
    define<Dec_>(Arrow(U, U), lambda([](const auto& P) { return Either(P, Arrow(P, Absurd)); }));
