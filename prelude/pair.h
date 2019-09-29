#pragma once

#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pair/sigma.h"
#include "pie_base.h"
#include "strings.h"

inline CONSTEXPR auto Pair = define<Pair_>(
    Arrow(U, U, U), lambda([](const auto& A, const auto& D) { return Sigma(A, [D](const auto&) { return D; }); }));
