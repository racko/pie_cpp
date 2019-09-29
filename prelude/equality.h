#pragma once

#include "eq/eq.h"
#include "eq/ind_eq.h"
#include "eq/same.h"
#include "lambda/app.h"
#include "lambda/lambda.h"
#include "lambda/pi.h"
#include "pie_base.h"
#include "strings.h"

inline CONSTEXPR auto replace = define<Replace>(
    Pi(U,
       [](const auto& X) {
           return Pi(X, [X](const auto& from) {
               return Pi(X, [from, X](const auto& to) {
                   return Arrow(Eq(X, from, to),
                                Pi(Arrow(X, U), [from, to](const auto& mot) { return Arrow(mot(from), mot(to)); }));
               });
           });
       }),
    lambda(
        [](const auto& /*X*/, const auto& /*from*/, const auto& /*to*/, const auto& t, const auto& m, const auto& b) {
            return ind_Eq(t, lambda([m](const auto& x, const auto& /*proof_from*/) { return m(x); }), b);
        }));

inline CONSTEXPR auto cong = define<Cong>(
    Pi(U,
       [](const auto& X) {
           return Pi(U, [X](const auto& Y) {
               return Pi(X, [X, Y](const auto& from) {
                   return Pi(X, [X, Y, from](const auto& to) {
                       return Arrow(Eq(X, from, to),
                                    Pi(Arrow(X, Y), [Y, from, to](const auto& f) { return Eq(Y, f(from), f(to)); }));
                   });
               });
           });
       }),
    lambda([](const auto& /*X*/, const auto& Y, const auto& from, const auto& /*to*/, const auto& t, const auto& f) {
        return ind_Eq(
            t, lambda([Y, f, from](const auto& x, const auto& /*t*/) { return Eq(Y, f(from), f(x)); }), same(f(from)));
    }));
