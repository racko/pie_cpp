#include "prelude.h"

#include <catch2/catch.hpp>
#include <codecvt>
#include <iostream>
#include <locale>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

// std::wbuffer_convert<std::codecvt_utf8<wchar_t>> cout_conv(std::cout.rdbuf());
// std::wbuffer_convert<std::codecvt_utf8<wchar_t>> cerr_conv(std::cerr.rdbuf());

TEST_CASE("Chapter2", "") {
    SECTION("lambda") {
        CHECK(IsAValue(Arrow(Atom, Pair(Atom, Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote(L"lentils")); })));
        CHECK(IsTheValueOf(Pair(Atom, Atom), cons(quote(L"garlic"), quote(L"lentils")), lambda([](auto flavor) {
                               return cons(flavor, quote(L"lentils"));
                           })(quote(L"garlic"))));
        CHECK(IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)),
            cons(quote(L"potato"), cons(plus(nat<1>(), nat<2>()), quote(L"potato"))),
            lambda([](auto root) { return cons(root, cons(plus(nat<1>(), nat<2>()), root)); })(quote(L"potato"))));
        CHECK(!IsTheValueOf(
            Pair(Atom, Pair(Nat, Atom)),
            cons(quote(L"potato"), cons(nat<3>(), quote(L"potato"))),
            lambda([](auto root) { return cons(root, cons(plus(nat<1>(), nat<2>()), root)); })(quote(L"potato"))));
        CHECK(IsTheValueOf(
            Pair(Atom, Arrow(Atom, Atom)),
            cons(quote(L"carrot"), lambda([](auto root) { return root; })),
            lambda([](auto root) { return cons(root, lambda([](auto root) { return root; })); })(quote(L"carrot"))));
        CHECK(IsAValue(Arrow(car(cons(Atom, quote(L"pepper"))), Pair(cdr(cons(quote(L"salt"), Atom)), Atom)),
                       lambda([](auto flavor) { return cons(flavor, quote(L"lentils")); })));
        CHECK(IsTheSameAs(Arrow(Nat, Pair(Nat, Nat)), lambda([](auto x) { return cons(x, x); }), lambda([](auto y) {
                              return cons(y, y);
                          })));
        CHECK(!IsTheSameAs(Arrow(Atom, Arrow(Atom, Pair(Atom, Atom))),
                           lambda([](auto a) { return lambda([a](auto d) { return cons(a, d); }); }),
                           lambda([](auto d) { return lambda([d](auto a) { return cons(a, d); }); })));
        CHECK(IsTheSameAs(
            Arrow(Nat, Nat), lambda([](auto y) { return car(cons(y, y)); }), lambda([](auto x) { return x; })));
    }

    SECTION("neutral") {
        CHECK(!IsNeutral(cons(var(), quote(L"rutabaga"))));

        // page 40, frame 26: "(cons y 'rutabaga) is a value" ... but I have to provide a type :/
        // CHECK(IsAValue(Pair(Nat, Atom), cons(var(), quote(L"rutabaga"))));

        CHECK(IsNeutral(cdr(var())));

        CHECK(IsTheSameAs(
            Arrow(Pair(Nat, Nat), Nat), lambda([](auto x) { return car(x); }), lambda([](auto y) { return car(y); })));
    }

    SECTION("claim and define") {
        const auto vegetables = define(L"vegetables", Pair(Atom, Atom), cons(quote(L"celery"), quote(L"carrot")));
        CHECK(IsTheSameAs(Pair(Atom, Atom), vegetables, cons(car(vegetables), cdr(vegetables))));
        // TODO This should also work with an undefined variable
        // TODO fix the same for lambdas (eta-expansion)
    }

    SECTION("which-Nat") {
        CHECK(ComputeValue(which_Nat(Atom, four, quote(L"naught"), lambda([](auto) { return quote(L"more"); }))) ==
              quote(L"more"));
        CHECK(Normalize(which_Nat(Nat, five, zero, lambda([](auto n) { return plus(six, n); }))) == ten);
        // gauss cannot be implemented yet (and isn't defined in the book ... yet)
    }

    SECTION("type values") {
        CHECK(AreTheSameType(car(cons(Atom, quote(L"prune"))), Atom));
        CHECK(!IsA(cons(Atom, Atom), U));
        CHECK(IsA(cons(Atom, Atom), Pair(U, U)));
        CHECK(!IsA(U, U));
        CHECK(IsA(cons(Atom, Nat), Pair(U, U)));
        // const auto Pear = define(L"Pear", U, Pair(Nat, Nat));
        // TODO page 56 - 60 requires context arguments to functions ...
    }
}
