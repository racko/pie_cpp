#pragma once

#include "pie.h"

inline CONSTEXPR auto one{define("one", Nat, add1(zero))};
inline CONSTEXPR auto two{define("two", Nat, add1(one))};
inline CONSTEXPR auto three{define("three", Nat, add1(two))};
inline CONSTEXPR auto four{define("four", Nat, add1(three))};
inline CONSTEXPR auto five{define("five", Nat, add1(four))};
inline CONSTEXPR auto six{define("six", Nat, add1(five))};
inline CONSTEXPR auto seven{define("seven", Nat, add1(six))};
inline CONSTEXPR auto eight{define("eight", Nat, add1(seven))};
inline CONSTEXPR auto nine{define("nine", Nat, add1(eight))};
inline CONSTEXPR auto ten{define("ten", Nat, add1(nine))};

inline CONSTEXPR auto Pair = define("Pair", Arrow(U, U, U), lambda([](const auto& A, const auto& D) {
                                        return Sigma(A, [D](const auto&) { return D; });
                                    }));

inline CONSTEXPR auto Maybe = define("Maybe", Arrow(U, U), lambda([](const auto& X) { return Either(X, Trivial); }));

inline CONSTEXPR auto nothing =
    define("nothing", Pi(U, [](const auto& E) { return Maybe(E); }), lambda([](const auto&) { return right(sole); }));

inline CONSTEXPR auto just = define("just",
                                    Pi(U, [](const auto& E) { return Arrow(E, Maybe(E)); }),
                                    lambda([](const auto&, const auto& x) { return left(x); }));

inline CONSTEXPR auto Fin = define("Fin", Arrow(Nat, U), lambda([](const auto& n) {
                                       return ind_Nat(n,
                                                      lambda([](const auto&) { return U; }),
                                                      Absurd,
                                                      lambda([](const auto&, const auto& acc) { return Maybe(acc); }));
                                   }));

inline CONSTEXPR auto fzero = define("fzero",
                                     Pi(Nat, [](const auto& n) { return Fin(add1(n)); }),
                                     lambda([](const auto& n) { return nothing(Fin(n)); }));

inline CONSTEXPR auto fadd1 = define("fadd1",
                                     Pi(Nat, [](const auto& n) { return Arrow(Fin(n), Fin(add1(n))); }),
                                     lambda([](const auto& n, const auto& smaller) { return just(Fin(n), smaller); }));

inline CONSTEXPR auto Dec =
    define("Dec", Arrow(U, U), lambda([](const auto& P) { return Either(P, Arrow(P, Absurd)); }));

inline CONSTEXPR auto replace = define(
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

inline CONSTEXPR auto cong = define(
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

inline CONSTEXPR auto constant =
    define("const",
           Pi(U, [](const auto& A) { return Pi(U, [A](const auto& B) { return Arrow(A, B, A); }); }),
           lambda([](const auto&, const auto&, const auto& a, const auto&) { return a; }));

inline CONSTEXPR auto flip = define(
    "flip",
    Pi(U,
       [](const auto& A) {
           return Pi(U, [A](const auto& B) {
               return Pi(U, [A, B](const auto& C) { return Arrow(Arrow(A, B, C), B, A, C); });
           });
       }),
    lambda([](const auto&, const auto&, const auto&, const auto& f, const auto& b, const auto& a) { return f(a, b); }));

inline CONSTEXPR auto const_ind_Mot =
    define("const-ind-Mot", Arrow(U, Nat, U), lambda([](const auto& X, const auto&) { return X; }));

inline CONSTEXPR auto iter_Nat =
    define("iter-Nat",
           Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(X, X), X); }),
           lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
               return ind_Nat(
                   target, const_ind_Mot(X), base, lambda([step](const auto&, const auto& acc) { return step(acc); }));
           }));

inline CONSTEXPR auto rec_Nat =
    define("rec-Nat",
           Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X, X), X); }),
           lambda([](const auto& X, const auto& target, const auto& base, const auto& step) {
               return ind_Nat(target, const_ind_Mot(X), base, step);
           }));

inline CONSTEXPR auto which_Nat = [] {
    CONSTEXPR auto step_which_Nat = define(
        "step-which-Nat",
        Pi(U, [](const auto& X) { return Arrow(Arrow(Nat, X), Nat, X, X); }),
        lambda([](const auto&, const auto& step, const auto& n_minus_1, const auto&) { return step(n_minus_1); }));

    return define("which-Nat",
                  Pi(U, [](const auto& X) { return Arrow(Nat, X, Arrow(Nat, X), X); }),
                  lambda([step_which_Nat](const auto& X, const auto& target, const auto& base, const auto& step) {
                      return rec_Nat(X, target, base, step_which_Nat(X, step));
                  }));
}();

inline CONSTEXPR auto plus = [] {
    CONSTEXPR auto step_plus = define("step-+", Arrow(Nat, Nat), lambda([](const auto& acc) { return add1(acc); }));

    return define("+", Arrow(Nat, Nat, Nat), lambda([step_plus](const auto& lhs, const auto& rhs) {
                      return iter_Nat(Nat, lhs, rhs, step_plus);
                  }));
}();

inline CONSTEXPR auto zerop = define("zerop", Arrow(Nat, Atom), lambda([](const auto& x) {
                                         return iter_Nat(Atom, x, quote("t"), constant(Atom, Atom, quote("nil")));
                                     }));

inline CONSTEXPR auto gauss = [] {
    CONSTEXPR auto step_gauss =
        define("step-gauss", Arrow(Nat, Nat, Nat), lambda([](const auto& smaller, const auto& acc) {
                   return plus(add1(smaller), acc);
               }));

    return define(
        "gauss", Arrow(Nat, Nat), lambda([step_gauss](const auto& x) { return rec_Nat(Nat, x, zero, step_gauss); }));
}();

inline CONSTEXPR auto times = define("*", Arrow(Nat, Nat, Nat), lambda([](const auto& lhs, const auto& rhs) {
                                         return iter_Nat(Nat, lhs, zero, plus(rhs));
                                     }));

inline CONSTEXPR auto incr =
    define("incr", Arrow(Nat, Nat), lambda([](const auto& n) { return iter_Nat(Nat, n, one, plus(one)); }));

inline CONSTEXPR auto plus1_equals_add1 = define("+1=add1",
                                                 Pi(Nat, [](const auto& n) { return Eq(Nat, plus(one, n), add1(n)); }),
                                                 lambda([](const auto& n) { return same(add1(n)); }));

inline CONSTEXPR auto incr_equals_add1 = [] {
    CONSTEXPR auto base_incr_equals_add1 = define("base-incr=add1", Eq(Nat, incr(zero), add1(zero)), same(add1(zero)));

    CONSTEXPR auto mot_incr_equals_add1 =
        define("mot-incr=add1", Arrow(Nat, U), lambda([](const auto& k) { return Eq(Nat, incr(k), add1(k)); }));

    CONSTEXPR auto step_incr_equals_add1 = define(
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

// CONSTEXPR auto doub = define("double", Arrow(Nat, Nat), lambda([](const auto& n) { return times(n, two); }));
inline CONSTEXPR auto doub = define("double", Arrow(Nat, Nat), flip(Nat, Nat, Nat, times, two));

inline CONSTEXPR auto twice = define("twice", Arrow(Nat, Nat), lambda([](const auto& n) { return plus(n, n); }));

inline CONSTEXPR auto Even = define("Even", Arrow(Nat, U), lambda([](const auto& n) {
                                        return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, doub(half)); });
                                    }));

inline CONSTEXPR auto Odd =
    define("Odd", Arrow(Nat, U), lambda([](const auto& n) {
               return Sigma(Nat, [n](const auto& half) { return Eq(Nat, n, add1(doub(half))); });
           }));

inline CONSTEXPR auto zero_is_even = define("zero-is-even", Even(zero), cons(zero, same(zero)));

inline CONSTEXPR auto plus_two_even =
    define("+two-even",
           Pi(Nat, [](const auto& n) { return Arrow(Even(n), Even(plus(two, n))); }),
           lambda([](const auto& n, const auto& n_even) {
               return cons(add1(car(n_even)), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(two)));
           }));

inline CONSTEXPR auto two_is_even = define("two-is-even", Even(two), plus_two_even(zero, zero_is_even));

inline CONSTEXPR auto one_is_odd = define("one-is-odd", Odd(one), cons(zero, same(one)));

inline CONSTEXPR auto even_implies_add1_odd =
    define("even->add1-odd",
           Pi(Nat, [](const auto& n) { return Arrow(Even(n), Odd(add1(n))); }),
           lambda([](const auto& n, const auto& n_even) {
               return cons(car(n_even), cong(Nat, Nat, n, doub(car(n_even)), cdr(n_even), plus(one)));
           }));

inline CONSTEXPR auto odd_implies_add1_even =
    define("odd->add1-even",
           Pi(Nat, [](const auto& n) { return Arrow(Odd(n), Even(add1(n))); }),
           lambda([](const auto& n, const auto& n_odd) {
               return cons(add1(car(n_odd)), cong(Nat, Nat, n, add1(doub(car(n_odd))), cdr(n_odd), plus(one)));
           }));

inline CONSTEXPR auto even_or_odd = [] {
    CONSTEXPR auto mot_even_or_odd =
        define("mot-even-or-odd", Arrow(Nat, U), lambda([](const auto& k) { return Either(Even(k), Odd(k)); }));

    CONSTEXPR auto base_even_or_odd = define("base-even-or-odd", mot_even_or_odd(zero), left(zero_is_even));

    CONSTEXPR auto step_even_or_odd = [mot_even_or_odd] {
        CONSTEXPR auto mot_step_even_or_odd = define(
            "mot-step-even-or-odd",
            Pi(Nat, [](const auto& n) { return Arrow(Either(Even(n), Odd(n)), U); }),
            lambda([](const auto& n, const auto& /*n_even_or_odd*/) { return Either(Even(add1(n)), Odd(add1(n))); }));

        CONSTEXPR auto base_left_step_even_or_odd =
            define("base-left-step-even-or-odd",
                   Pi(Nat,
                      [mot_step_even_or_odd](const auto& n) {
                          return Pi(Even(n), [mot_step_even_or_odd, n](const auto& n_even) {
                              return mot_step_even_or_odd(n, left(n_even));
                          });
                      }),
                   lambda([](const auto& n, const auto& n_even) { return right(even_implies_add1_odd(n, n_even)); }));

        CONSTEXPR auto base_right_step_even_or_odd =
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

inline CONSTEXPR auto use_Nat_eq = [] {
    CONSTEXPR auto eq_consequence =
        define("=consequence", Arrow(Nat, Nat, U), lambda([](const auto& n, const auto& j) {
                   return ind_Nat(
                       n,
                       lambda([](const auto&) { return U; }),
                       ind_Nat(j, lambda([](const auto&) { return U; }), Trivial, lambda([](const auto&, const auto&) {
                                   return Absurd;
                               })),
                       lambda([j](const auto& n_minus_1, const auto&) {
                           return ind_Nat(j,
                                          lambda([](const auto&) { return U; }),
                                          Absurd,
                                          lambda([n_minus_1](const auto& j_minus_1, const auto&) {
                                              return Eq(Nat, n_minus_1, j_minus_1);
                                          }));
                       }));
               }));

    CONSTEXPR auto eq_consequence_same =
        define("=consequence-same",
               Pi(Nat, [eq_consequence](const auto& n) { return eq_consequence(n, n); }),
               lambda([eq_consequence](const auto& n) {
                   return ind_Nat(n,
                                  lambda([eq_consequence](const auto& k) { return eq_consequence(k, k); }),
                                  sole,
                                  lambda([](const auto& n_minus_1, const auto&) { return same(n_minus_1); }));
               }));

    return define("use-Nat-eq",
                  Pi(Nat,
                     [eq_consequence](const auto& n) {
                         return Pi(Nat, [eq_consequence, n](const auto& j) {
                             return Arrow(Eq(Nat, n, j), eq_consequence(n, j));
                         });
                     }),
                  lambda([eq_consequence, eq_consequence_same](const auto& n, const auto& j, const auto& n_equals_j) {
                      return replace(Nat,
                                     n,
                                     j,
                                     n_equals_j,
                                     lambda([eq_consequence, n](const auto& k) { return eq_consequence(n, k); }),
                                     eq_consequence_same(n));
                  }));
}();

inline CONSTEXPR auto zero_not_add1 =
    define("zero-not-add1",
           Pi(Nat, [](const auto& n) { return Arrow(Eq(Nat, zero, add1(n)), Absurd); }),
           lambda([](const auto& n) { return use_Nat_eq(zero, add1(n)); }));
inline CONSTEXPR auto sub1_eq =
    define("sub1=",
           Pi(Nat,
              [](const auto& n) {
                  return Pi(Nat, [n](const auto& j) { return Arrow(Eq(Nat, add1(n), add1(j)), Eq(Nat, n, j)); });
              }),
           lambda([](const auto& n, const auto& j) { return use_Nat_eq(add1(n), add1(j)); }));

inline CONSTEXPR auto is_zero = define(
    "zero?", Pi(Nat, [](const auto& n) { return Dec(Eq(Nat, zero, n)); }), lambda([](const auto& n) {
        return ind_Nat(n,
                       lambda([](const auto& k) { return Dec(Eq(Nat, zero, k)); }),
                       left(same(zero)),
                       lambda([](const auto& n_minus_1, const auto&) { return right(zero_not_add1(n_minus_1)); }));
    }));

namespace church {
inline CONSTEXPR auto Nat = define("Nat", U, Pi(U, [](const auto& X) { return Arrow(X, Arrow(X, X), X); }));
inline CONSTEXPR auto zero = define("zero", Nat, lambda([](const auto&, const auto& b, const auto&) { return b; }));
inline CONSTEXPR auto add1 =
    define("add1", Arrow(Nat, Nat), lambda([](const auto& smaller, const auto& X, const auto& b, const auto& step) {
               return step(smaller(X, b, step));
           }));
} // namespace church
