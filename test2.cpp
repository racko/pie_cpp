#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct quote_garlic {
    static inline constexpr const char* value = "garlic";
};

struct quote_potato {
    static inline constexpr const char* value = "potato";
};

struct quote_lentils {
    static inline constexpr const char* value = "lentils";
};

struct quote_pepper {
    static inline constexpr const char* value = "pepper";
};

struct quote_salt {
    static inline constexpr const char* value = "salt";
};

struct quote_rutabaga {
    static inline constexpr const char* value = "rutabaga";
};

struct quote_celery {
    static inline constexpr const char* value = "celery";
};

struct quote_carrot {
    static inline constexpr const char* value = "carrot";
};

struct quote_naught {
    static inline constexpr const char* value = "naught";
};

struct quote_more {
    static inline constexpr const char* value = "more";
};

struct quote_prune {
    static inline constexpr const char* value = "prune";
};

TEST_CASE("Chapter2", "") {
    SECTION("lambda") {
        CHECK(IsAValue(Arrow(Atom, Pair(Atom, Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote<quote_lentils>()); })));
        CHECK(
            IsTheValueOf(Pair(Atom, Atom), cons(quote<quote_garlic>(), quote<quote_lentils>()), lambda([](auto flavor) {
                             return cons(flavor, quote<quote_lentils>());
                         })(quote<quote_garlic>())));
        CHECK(IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)),
            cons(quote<quote_potato>(), cons(plus(nat<1>(), nat<2>()), quote<quote_potato>())),
            lambda([](auto root) { return cons(root, cons(plus(nat<1>(), nat<2>()), root)); })(quote<quote_potato>())));
        CHECK(!IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)),
            cons(quote<quote_potato>(), cons(nat<3>(), quote<quote_potato>())),
            lambda([](auto root) { return cons(root, cons(plus(nat<1>(), nat<2>()), root)); })(quote<quote_potato>())));
        // here we would want to check whether shadowing works as expected, but
        // a) we use C++ variables/lambdas, so "it works".
        // b) We even get shadowing warnings which prevent compilation due to -Werror :rofl:
        CHECK(IsTheValueOf(Pair(Atom, Arrow(Atom, Atom)),
                           cons(quote<quote_carrot>(), lambda([](auto root) { return root; })),
                           lambda([](auto root) { return cons(root, lambda([](auto root1) { return root1; })); })(
                               quote<quote_carrot>())));
        CHECK(IsAValue(Arrow(car(cons(Atom, quote<quote_pepper>())), Pair(cdr(cons(quote<quote_salt>(), Atom)), Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote<quote_lentils>()); })));
        CHECK(IsTheSameAs(Arrow(Nat, Pair(Nat, Nat)), lambda([](auto x) { return cons(x, x); }), lambda([](auto y) {
                              return cons(y, y);
                          })));
        CHECK(!IsTheSameAs(Arrow(Atom, Arrow(Atom, Pair(Atom, Atom))),
                           lambda([](auto a) { return lambda([a](auto d) { return cons(a, d); }); }),
                           lambda([](auto d) { return lambda([d](auto a) { return cons(a, d); }); })));
        CHECK(IsTheSameAs(Arrow(Nat, Nat),
                          lambda([](auto y) { return car(the(Pair(Nat, Nat), cons(y, y))); }),
                          lambda([](auto x) { return x; })));
    }

    SECTION("neutral") {
        CHECK(!IsNeutral(cons(var<0>(), quote<quote_rutabaga>())));

        // page 40, frame 26: "(cons y 'rutabaga) is a value" ... but I have to provide a type :/
        // CHECK(IsAValue(Pair(Nat, Atom), cons(var(), quote<quote_rutabaga>())));

        CHECK(IsNeutral(cdr(var<0>())));

        CHECK(IsTheSameAs(
            Arrow(Pair(Nat, Nat), Nat), lambda([](auto x) { return car(x); }), lambda([](auto y) { return car(y); })));
    }

    SECTION("claim and define") {
        const auto vegetables =
            define("vegetables", Pair(Atom, Atom), cons(quote<quote_celery>(), quote<quote_carrot>()));
        CHECK(IsTheSameAs(Pair(Atom, Atom), vegetables, cons(car(vegetables), cdr(vegetables))));
        // TODO This should also work with an undefined variable
        // TODO fix the same for lambdas (eta-expansion)
    }

    SECTION("which-Nat") {
        CHECK(ComputeValue(which_Nat(Atom, four, quote<quote_naught>(), lambda([](auto) {
                                         return quote<quote_more>();
                                     }))) == quote<quote_more>());
        CHECK(Normalize(which_Nat(Nat, five, zero, lambda([](auto n) { return plus(six, n); }))) == ten);
        // gauss cannot be implemented yet (and isn't defined in the book ... yet)
    }

    SECTION("type values") {
        CHECK(AreTheSameType(car(cons(Atom, quote<quote_prune>())), Atom));
        CHECK(!IsA(cons(Atom, Atom), U));
        CHECK(IsA(cons(Atom, Atom), Pair(U, U)));
        CHECK(!IsA(U, U));
        CHECK(IsA(cons(Atom, Nat), Pair(U, U)));
        // const auto Pear = define("Pear", U, Pair(Nat, Nat));
        // TODO page 56 - 60 requires context arguments to functions ...
    }
}
