#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct Garlic {
    static inline constexpr const char* value = "garlic";
};

struct Potato {
    static inline constexpr const char* value = "potato";
};

struct Lentils {
    static inline constexpr const char* value = "lentils";
};

struct Pepper {
    static inline constexpr const char* value = "pepper";
};

struct Salt {
    static inline constexpr const char* value = "salt";
};

struct Rutabaga {
    static inline constexpr const char* value = "rutabaga";
};

struct Celery {
    static inline constexpr const char* value = "celery";
};

struct Carrot {
    static inline constexpr const char* value = "carrot";
};

struct Naught {
    static inline constexpr const char* value = "naught";
};

struct More {
    static inline constexpr const char* value = "more";
};

struct Prune {
    static inline constexpr const char* value = "prune";
};

struct Vegetables {
    static inline constexpr const char* value = "vegetables";
};

TEST_CASE("Chapter2", "") {
    SECTION("lambda") {
        CHECK(IsAValue(Arrow(Atom, Pair(Atom, Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote<Lentils>()); })));
        CHECK(IsTheValueOf(Pair(Atom, Atom), cons(quote<Garlic>(), quote<Lentils>()), lambda([](auto flavor) {
                               return cons(flavor, quote<Lentils>());
                           })(quote<Garlic>())));
        CHECK(IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)),
            cons(quote<Potato>(), cons(plus(nat<1>(), nat<2>()), quote<Potato>())),
            lambda([](auto root) { return cons(root, cons(plus(nat<1>(), nat<2>()), root)); })(quote<Potato>())));
        CHECK(!IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)), cons(quote<Potato>(), cons(nat<3>(), quote<Potato>())), lambda([](auto root) {
                return cons(root, cons(plus(nat<1>(), nat<2>()), root));
            })(quote<Potato>())));
        // here we would want to check whether shadowing works as expected, but
        // a) we use C++ variables/lambdas, so "it works".
        // b) We even get shadowing warnings which prevent compilation due to -Werror :rofl:
        CHECK(IsTheValueOf(
            Pair(Atom, Arrow(Atom, Atom)),
            cons(quote<Carrot>(), lambda([](auto root) { return root; })),
            lambda([](auto root) { return cons(root, lambda([](auto root1) { return root1; })); })(quote<Carrot>())));
        CHECK(IsAValue(Arrow(car(cons(Atom, quote<Pepper>())), Pair(cdr(cons(quote<Salt>(), Atom)), Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote<Lentils>()); })));
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
        CHECK(!IsNeutral(cons(var<0>(), quote<Rutabaga>())));

        // page 40, frame 26: "(cons y 'rutabaga) is a value" ... but I have to provide a type :/
        // CHECK(IsAValue(Pair(Nat, Atom), cons(var(), quote<Rutabaga>())));

        CHECK(IsNeutral(cdr(var<0>())));

        CHECK(IsTheSameAs(
            Arrow(Pair(Nat, Nat), Nat), lambda([](auto x) { return car(x); }), lambda([](auto y) { return car(y); })));
    }

    SECTION("claim and define") {
        const auto vegetables = define<Vegetables>(Pair(Atom, Atom), cons(quote<Celery>(), quote<Carrot>()));
        CHECK(IsTheSameAs(Pair(Atom, Atom), vegetables, cons(car(vegetables), cdr(vegetables))));
        // TODO This should also work with an undefined variable
        // TODO fix the same for lambdas (eta-expansion)
    }

    SECTION("which-Nat") {
        CHECK(ComputeValue(which_Nat(Atom, four, quote<Naught>(), lambda([](auto) { return quote<More>(); }))) ==
              quote<More>());
        CHECK(Normalize(which_Nat(Nat, five, zero, lambda([](auto n) { return plus(six, n); }))) == ten);
        // gauss cannot be implemented yet (and isn't defined in the book ... yet)
    }

    SECTION("type values") {
        CHECK(AreTheSameType(car(cons(Atom, quote<Prune>())), Atom));
        CHECK(!IsA(cons(Atom, Atom), U));
        CHECK(IsA(cons(Atom, Atom), Pair(U, U)));
        CHECK(!IsA(U, U));
        CHECK(IsA(cons(Atom, Nat), Pair(U, U)));
        // const auto Pear = define("Pear", U, Pair(Nat, Nat));
        // TODO page 56 - 60 requires context arguments to functions ...
    }
}
