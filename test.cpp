#include "pie.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter1", "") {
    SECTION("IsA Atom") {
        CHECK(IsA(quote(L"atom"), Atom));
        CHECK(IsA(quote(L"ratatouille"), Atom));
        CHECK(IsA(quote(L"is-it-obvious-that-this-is-an-atom"), Atom));
        CHECK(IsA(quote(L"----"), Atom));
        CHECK(IsA(quote(L"---"), Atom));
        // CHECK(!IsA(L"---", Atom));
        CHECK(!IsA(quote(L""), Atom));
        CHECK(IsA(quote(L"Atom"), Atom));
        CHECK(!IsA(quote(L"at0m"), Atom));
        CHECK(IsA(quote(L"cœur-d-artichauts"), Atom));
        CHECK(IsA(quote(L"ἄτομον"), Atom));
        CHECK(IsA(quote(L"courgette"), Atom));
    }

    SECTION("IsTheSameAs Atom") {
        CHECK(IsTheSameAs(Atom, quote(L"ratatouille"), quote(L"ratatouille")));
        CHECK(!IsTheSameAs(Atom, quote(L"ratatouille"), quote(L"baguette")));
        CHECK(IsTheSameAs(Atom, quote(L"citron"), quote(L"citron")));
        CHECK(!IsTheSameAs(Atom, quote(L"pomme"), quote(L"orange")));
    }

    SECTION("IsA Pair") { CHECK(IsA(cons(quote(L"ratatouille"), quote(L"baguette")), Pair(Atom, Atom))); }

    SECTION("IsTheSameAs Pair") {
        CHECK(IsTheSameAs(Pair(Atom, Atom),
                          cons(quote(L"ratatouille"), quote(L"baguette")),
                          cons(quote(L"ratatouille"), quote(L"baguette"))));
        CHECK(!IsTheSameAs(Pair(Atom, Atom),
                           cons(quote(L"ratatouille"), quote(L"baguette")),
                           cons(quote(L"baguette"), quote(L"baguette"))));
    }

    SECTION("IsAType") {
        CHECK(IsAType(Atom));
        CHECK(IsAType(Pair(Atom, Atom)));
        CHECK(!IsAType(quote(L"courgette")));
    }

    SECTION("AreTheSameType") {
        CHECK(AreTheSameType(Atom, Atom));
        CHECK(!AreTheSameType(Atom, Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, Atom), Pair(Atom, Atom)));
    }

    SECTION("Presuppositions") { CHECK(!IsTheSameAs(quote(L"fruit"), quote(L"pêche"), quote(L"pêche"))); }

    SECTION("car and cdr") {
        CHECK(IsTheSameAs(Atom, car(cons(quote(L"ratatouille"), quote(L"baguette"))), quote(L"ratatouille")));
        CHECK(IsTheSameAs(Atom, cdr(cons(quote(L"ratatouille"), quote(L"baguette"))), quote(L"baguette")));
        CHECK(IsA(car(cons(cons(quote(L"aubergine"), quote(L"courgette")), quote(L"tomato"))), Pair(Atom, Atom)));
        CHECK(IsTheSameAs(Atom,
                          car(cdr(cons(quote(L"ratatouille"), cons(quote(L"baguette"), quote(L"olive-oil"))))),
                          quote(L"baguette")));
    }

    SECTION("normal forms") {
        CHECK(IsNormalFormOf(Atom,
                             quote(L"olive-oil"),
                             cdr(cdr(cons(quote(L"ratatouille"), cons(quote(L"baguette"), quote(L"olive-oil")))))));
        CHECK(IsANormal(cons(quote(L"ratatouille"), quote(L"baguette")), Pair(Atom, Atom)));
        CHECK(IsNormalFormOf(Pair(Atom, Atom),
                             cons(quote(L"aubergine"), quote(L"courgette")),
                             car(cons(cons(quote(L"aubergine"), quote(L"courgette")), quote(L"tomato")))));
        CHECK(IsTheSameAs(Pair(Atom, Atom),
                          car(cons(cons(quote(L"aubergine"), quote(L"courgette")), quote(L"tomato"))),
                          cons(quote(L"aubergine"), quote(L"courgette"))));
        CHECK(IsTheSameAs(Pair(Atom, Atom),
                          car(cons(cons(quote(L"aubergine"), quote(L"courgette")), quote(L"tomato"))),
                          car(cons(cons(quote(L"aubergine"), quote(L"courgette")), quote(L"tomato")))));
        CHECK(IsIllTyped(Pair(cdr(cons(Atom, quote(L"olive"))), car(cons(quote(L"oil"), Atom)))));
        CHECK(IsNormalFormOfType(Pair(car(cons(Atom, quote(L"olive"))), cdr(cons(quote(L"oil"), Atom))),
                                 Pair(Atom, Atom)));
        CHECK(IsA(cons(quote(L"ratatouille"), quote(L"baguette")),
                  Pair(car(cons(Atom, quote(L"olive"))), cdr(cons(quote(L"oil"), Atom)))));
        CHECK(AreTheSameType(Pair(car(cons(Atom, quote(L"olive"))), cdr(cons(quote(L"oil"), Atom))), Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, cdr(cons(quote(L"oil"), Atom))), Pair(Atom, Atom)));
    }

    SECTION("Nat") {
        CHECK(!IsA(quote(L"6"), Atom));
        CHECK(!IsA(cons(quote(L"17"), quote(L"pepper")), Pair(Atom, Atom)));
        CHECK(IsA(nat(1), Nat));
        CHECK(IsA(nat(1729), Nat));
        CHECK(!IsA(nat(-1), Nat));
        CHECK(!IsA(nat(-23), Nat));
        CHECK(IsA(nat(0), Nat));
        CHECK(!IsTheSameAs(Nat, nat(0), nat(26)));
        CHECK(IsTheSameAs(Nat, plus(nat(0), nat(26)), nat(26)));
        CHECK(IsTheSameAs(Nat, nat(0), zero));
        CHECK(IsTheSameAs(Nat, nat(1), one));
        CHECK(IsTheSameAs(Nat, nat(2), two));
        CHECK(IsTheSameAs(Nat, nat(4), four));
        CHECK(IsTheSameAs(Nat, nat(8), eight));
        CHECK(IsANormal(nat(8), Nat));
        CHECK(IsANormal(eight, Nat));
        CHECK(IsANormal(nat(7), Nat));
        CHECK(IsANormal(seven, Nat));
        CHECK(IsANormal(zero, Nat));
        CHECK(IsANormal(add1(zero), Nat));
    }

    SECTION("values") {
        CHECK(!IsANormal(add1(plus(add1(zero), add1(add1(zero)))), Nat));
        CHECK(IsAValue(Nat, add1(plus(add1(zero), add1(add1(zero))))));
        CHECK(!IsANormal(plus(add1(add1(zero)), add1(zero)), Nat));
        CHECK(!IsANormal(add1(plus(add1(zero), add1(one))), Nat));
        CHECK(!IsAValue(Nat, add1(quote(L"aubergine"))));
        CHECK(IsTheSameAs(Nat, add1(plus(add1(zero), add1(add1(zero)))), four));
        CHECK(IsTheSameAs(Nat, add1(zero), add1(zero)));
        CHECK(IsTheSameAs(Nat, add1(plus(nat(0), nat(1))), add1(plus(nat(1), nat(0)))));
        CHECK(IsTheSameAs(Nat, car(cons(plus(nat(3), nat(5)), quote(L"baguette"))), eight));
        CHECK(IsA(cons(zero, quote(L"onion")), Pair(Nat, Atom)));
        CHECK(IsA(cons(quote(L"zero"), quote(L"onion")), Pair(Atom, Atom)));
        CHECK(IsA(cons(quote(L"basil"), cons(quote(L"thyme"), quote(L"oregano"))), Pair(Atom, Pair(Atom, Atom))));
    }

    // TODO: Evaluation results in values. Expressions that aren't values are the same if their values are the same.
}
