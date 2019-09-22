#pragma once

#include "pie.h"

FOO auto one{define("one", Nat, add1(zero))};
FOO auto two{define("two", Nat, add1(one))};
FOO auto three{define("three", Nat, add1(two))};
FOO auto four{define("four", Nat, add1(three))};
FOO auto five{define("five", Nat, add1(four))};
FOO auto six{define("six", Nat, add1(five))};
FOO auto seven{define("seven", Nat, add1(six))};
FOO auto eight{define("eight", Nat, add1(seven))};
FOO auto nine{define("nine", Nat, add1(eight))};
FOO auto ten{define("ten", Nat, add1(nine))};

// NOTE: NEVER STORE PIE<T> IN A LAMBDA. OR ANYWHERE FOR WHAT IT'S WORTH. SLICING!
// USE [x = x.derived] OR SIMILAR.

FOO auto Pair = define("Pair", Arrow(U, U, U), lambda([](const auto& A, const auto& D) {
                           return Sigma(A, [D](const auto&) { return D; });
                       }));

FOO auto replace = define(
    "replace",
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

FOO auto cong = define(
    "cong",
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

FOO auto constant = define("const",
                           Pi(U, [](const auto& A) { return Pi(U, [A](const auto& B) { return Arrow(A, B, A); }); }),
                           lambda([](const auto&, const auto&, const auto& a, const auto&) { return a; }));

FOO auto flip = define(
    "flip",
    Pi(U,
       [](const auto& A) {
           return Pi(U, [A](const auto& B) {
               return Pi(U, [A, B](const auto& C) { return Arrow(Arrow(A, B, C), B, A, C); });
           });
       }),
    lambda([](const auto&, const auto&, const auto&, const auto& f, const auto& b, const auto& a) { return f(a, b); }));

FOO auto const_ind_Mot =
    define("const-ind-Mot", Arrow(U, Nat, U), lambda([](const auto& X, const auto&) { return X; }));

FOO auto iter_Nat =
    define("iter-Nat",
           Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(X, X), X); }),
           lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
               return ind_Nat(
                   target, const_ind_Mot(X), base, lambda([step](const auto&, const auto& acc) { return step(acc); }));
           }));

FOO auto rec_Nat = define("rec-Nat",
                          Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
                          lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
                              return ind_Nat(target, const_ind_Mot(X), base, step);
                          }));

FOO auto which_Nat = [] {
    const auto step_which_Nat =
        define("step-which-Nat",
               Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
               lambda([](const auto&, const auto& step, const auto& acc, const auto&) { return step(acc); }));

    return define("which-Nat",
                  Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
                  lambda([step_which_Nat](const auto& X, const auto& target, const auto& base, const auto& step) {
                      return rec_Nat(X, target, base, step_which_Nat(X, step));
                  }));
}();

FOO auto plus = [] {
    const auto step_plus = define("step-+", Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

    return define("+", Arrow(Nat, Nat, Nat), lambda([step_plus](const auto& lhs, const auto& rhs) {
                      return iter_Nat(Nat, lhs, rhs, step_plus);
                  }));
}();

FOO auto zerop = define("zerop", Arrow(Nat, Atom), lambda([](const auto& x) {
                            return iter_Nat(Atom, x, quote("t"), constant(Atom, Atom, quote("nil")));
                        }));

FOO auto gauss = [] {
    const auto step_gauss = define("step-gauss", Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) {
                                       return plus(add1(smaller), acc);
                                   }));

    return define(
        "gauss", Arrow(Nat, Nat), lambda([step_gauss](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));
}();

FOO auto times = define("*", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) {
                            return iter_Nat(Nat, lhs, zero, plus(rhs));
                        }));

FOO auto incr = define("incr", Arrow(Nat, Nat), lambda([](const auto& n) { return iter_Nat(Nat, n, one, plus(one)); }));

FOO auto plus1_equals_add1 = define("+1=add1",
                                    Pi(Nat, [](const auto& n) { return Eq(Nat, plus(one, n), add1(n)); }),
                                    lambda([](const auto& n) { return same(add1(n)); }));

FOO auto incr_equals_add1 = [] {
    const auto base_incr_equals_add1 = define("base-incr=add1", Eq(Nat, incr(zero), add1(zero)), same(add1(zero)));

    const auto mot_incr_equals_add1 =
        define("mot-incr=add1", Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, incr(k), add1(k)); }));

    const auto step_incr_equals_add1 = define(
        "step-incr=add1",
        Pi(Nat,
           [](const auto& smaller) {
               return Arrow(Eq(Nat, incr(smaller), add1(smaller)), Eq(Nat, add1(incr(smaller)), add1(add1(smaller))));
           }),
        lambda([](const auto& smaller, const auto& almost) {
            return cong(Nat, Nat, incr(smaller), add1(smaller), almost, plus(one));
        }));

    return define("incr=add1",
                  Pi(Nat, [](const auto& n) { return Eq(Nat, incr(n), add1(n)); }),
                  lambda([mot_incr_equals_add1, base_incr_equals_add1, step_incr_equals_add1](const auto& n) {
                      return ind_Nat(n, mot_incr_equals_add1, base_incr_equals_add1, step_incr_equals_add1);
                  }));
}();

//FOO auto doub = define("double", Arrow(Nat, Nat), lambda([](const auto& n) { return times(n, two); }));
FOO auto doub = define("double", Arrow(Nat, Nat), flip(Nat, Nat, Nat, times, two));

FOO auto twice = define("twice", Arrow(Nat, Nat), lambda([](const auto& n) { return plus(n, n); }));

FOO auto Even = define("Even", Arrow(Nat, U), lambda([](const auto& n) {
                           return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, doub(half)); });
                       }));

FOO auto Odd = define("Odd", Arrow(Nat, U), lambda([](const auto& n) {
                          return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, add1(doub(half))); });
                      }));

FOO auto zero_is_even = define("zero-is-even", Even(zero), cons(zero, same(zero)));

FOO auto plus_two_even =
    define("+two-even",
           Pi(Nat, [](const auto& n) { return Arrow(Even(n), Even(plus(two, n))); }),
           lambda([](const auto& n, const auto& n_even) {
               return cons(add1(car(n_even)), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(two)));
           }));

FOO auto two_is_even = define("two-is-even", Even(two), plus_two_even(zero, zero_is_even));

FOO auto one_is_odd = define("one-is-odd", Odd(one), cons(zero, same(one)));

FOO auto even_implies_add1_odd =
    define("even->add1-odd",
           Pi(Nat, [](const auto& n) { return Arrow(Even(n), Odd(add1(n))); }),
           lambda([](const auto& n, const auto& n_even) {
               return cons(car(n_even), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(one)));
           }));

FOO auto odd_implies_add1_even =
    define("odd->add1-even",
           Pi(Nat, [](const auto& n) { return Arrow(Odd(n), Even(add1(n))); }),
           lambda([](const auto& n, const auto& n_odd) {
               return cons(add1(car(n_odd)), cong(Nat, Nat, n, add1(doub(car(n_odd))), cdr(n_odd), plus(one)));
           }));

FOO auto even_or_odd = [] {
    const auto mot_even_or_odd =
        define("mot-even-or-odd", Arrow(Nat, U), lambda([](const auto& k) { return Either(Even(k), Odd(k)); }));

    const auto base_even_or_odd = define("base-even-or-odd", mot_even_or_odd(zero), left(zero_is_even));

    const auto step_even_or_odd = [mot_even_or_odd] {
        const auto mot_step_even_or_odd = define(
            "mot-step-even-or-odd",
            Pi(Nat, [](const auto& n) { return Arrow(Either(Even(n), Odd(n)), U); }),
            lambda([](const auto& n, const auto& /*n_even_or_odd*/) { return Either(Even(add1(n)), Odd(add1(n))); }));

        const auto base_left_step_even_or_odd =
            define("base-left-step-even-or-odd",
                   Pi(Nat,
                      [mot_step_even_or_odd](const auto& n) {
                          return Pi(Even(n), [mot_step_even_or_odd, n](const auto& n_even) {
                              return mot_step_even_or_odd(n, left(n_even));
                          });
                      }),
                   lambda([](const auto& n, const auto& n_even) { return right(even_implies_add1_odd(n, n_even)); }));

        const auto base_right_step_even_or_odd =
            define("base-right-step-even-or-odd",
                   Pi(Nat,
                      [mot_step_even_or_odd](const auto& n) {
                          return Pi(Odd(n), [mot_step_even_or_odd, n](const auto& n_odd) {
                              return mot_step_even_or_odd(n, right(n_odd));
                          });
                      }),
                   lambda([](const auto& n, const auto& n_odd) { return left(odd_implies_add1_even(n, n_odd)); }));

        return define(
            "step-even-or-odd",
            Pi(Nat, [mot_even_or_odd](const auto& n) { return Arrow(mot_even_or_odd(n), mot_even_or_odd(add1(n))); }),
            lambda([mot_step_even_or_odd, base_left_step_even_or_odd, base_right_step_even_or_odd](
                       const auto& n, const auto& n_even_or_odd) {
                return ind_Either(n_even_or_odd,
                                  mot_step_even_or_odd(n),
                                  base_left_step_even_or_odd(n),
                                  base_right_step_even_or_odd(n));
            }));
    }();

    return define("even-or-odd",
                  Pi(Nat, [](const auto& n) { return Either(Even(n), Odd(n)); }),
                  lambda([mot_even_or_odd, base_even_or_odd, step_even_or_odd](const auto& n) {
                      return ind_Nat(n, mot_even_or_odd, base_even_or_odd, step_even_or_odd);
                  }));
}();

namespace church {
FOO auto Nat = define("Nat", U, Pi(U, [](const auto& X) { return Arrow(X, Arrow(X, X), X); }));
FOO auto zero = define("zero", Nat, lambda([](const auto&, const auto& b, const auto&) { return b; }));
FOO auto add1 =
    define("add1", Arrow(Nat, Nat), lambda([](const auto& smaller, const auto& X, const auto& b, const auto& step) {
               return step(smaller(X, b, step));
           }));
} // namespace church
