#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

TEST_CASE("Chapter1", "") {
    SECTION("IsA Atom") {
        CHECK(IsA(quote("atom"), Atom));
        CHECK(IsA(quote("ratatouille"), Atom));
        CHECK(IsA(quote("is-it-obvious-that-this-is-an-atom"), Atom));
        CHECK(IsA(quote("----"), Atom));
        CHECK(IsA(quote("---"), Atom));
        // CHECK(!IsA("---", Atom));
        CHECK(!IsA(quote(""), Atom));
        CHECK(IsA(quote("Atom"), Atom));
        CHECK(!IsA(quote("at0m"), Atom));
        // CHECK(IsA(quote("cœur-d-artichauts"), Atom));
        // CHECK(IsA(quote("ἄτομον"), Atom));
        CHECK(IsA(quote("courgette"), Atom));
    }

    SECTION("IsTheSameAs Atom") {
        CHECK(IsTheSameAs(Atom, quote("ratatouille"), quote("ratatouille")));
        CHECK(!IsTheSameAs(Atom, quote("ratatouille"), quote("baguette")));
        CHECK(IsTheSameAs(Atom, quote("citron"), quote("citron")));
        CHECK(!IsTheSameAs(Atom, quote("pomme"), quote("orange")));
    }

    SECTION("IsA Pair") { CHECK(IsA(cons(quote("ratatouille"), quote("baguette")), Pair(Atom, Atom))); }

    SECTION("IsTheSameAs Pair") {
        CHECK(IsTheSameAs(Pair(Atom, Atom),
                          cons(quote("ratatouille"), quote("baguette")),
                          cons(quote("ratatouille"), quote("baguette"))));
        CHECK(!IsTheSameAs(Pair(Atom, Atom),
                           cons(quote("ratatouille"), quote("baguette")),
                           cons(quote("baguette"), quote("baguette"))));
    }

    SECTION("IsAType") {
        CHECK(IsAType(Atom));
        CHECK(IsAType(Pair(Atom, Atom)));
        CHECK(!IsAType(quote("courgette")));
    }

    SECTION("AreTheSameType") {
        CHECK(AreTheSameType(Atom, Atom));
        CHECK(!AreTheSameType(Atom, Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, Atom), Pair(Atom, Atom)));
    }

    // TODO: This asserts. How to test for that?
    // SECTION("Presuppositions") { CHECK(!IsTheSameAs(quote("fruit"), quote("pêche"), quote("pêche"))); }

    SECTION("car and cdr") {
        CHECK(IsTheSameAs(
            Atom, car(the(Pair(Atom, Atom), cons(quote("ratatouille"), quote("baguette")))), quote("ratatouille")));
        CHECK(IsTheSameAs(
            Atom, cdr(the(Pair(Atom, Atom), cons(quote("ratatouille"), quote("baguette")))), quote("baguette")));
        CHECK(IsA(
            car(the(Pair(Pair(Atom, Atom), Atom), cons(cons(quote("aubergine"), quote("courgette")), quote("tomato")))),
            Pair(Atom, Atom)));
        CHECK(IsTheSameAs(Atom,
                          car(cdr(the(Pair(Atom, Pair(Atom, Atom)),
                                      cons(quote("ratatouille"), cons(quote("baguette"), quote("olive-oil")))))),
                          quote("baguette")));
    }

    SECTION("normal forms") {
        CHECK(IsNormalFormOf(Atom,
                             quote("olive-oil"),
                             cdr(cdr(cons(quote("ratatouille"), cons(quote("baguette"), quote("olive-oil")))))));
        CHECK(IsANormal(cons(quote("ratatouille"), quote("baguette")), Pair(Atom, Atom)));
        CHECK(IsNormalFormOf(Pair(Atom, Atom),
                             cons(quote("aubergine"), quote("courgette")),
                             car(cons(cons(quote("aubergine"), quote("courgette")), quote("tomato")))));
        CHECK(IsTheSameAs(
            Pair(Atom, Atom),
            car(the(Pair(Pair(Atom, Atom), Atom), cons(cons(quote("aubergine"), quote("courgette")), quote("tomato")))),
            cons(quote("aubergine"), quote("courgette"))));
        CHECK(IsTheSameAs(
            Pair(Atom, Atom),
            car(the(Pair(Pair(Atom, Atom), Atom), cons(cons(quote("aubergine"), quote("courgette")), quote("tomato")))),
            car(the(Pair(Pair(Atom, Atom), Atom),
                    cons(cons(quote("aubergine"), quote("courgette")), quote("tomato"))))));
        // CHECK(IsIllTyped(Pair(cdr(cons(Atom, quote("olive"))), car(cons(quote("oil"), Atom)))));
        CHECK(IsNormalFormOfType(Normalize(Pair(Atom, Atom)),
                                 Pair(car(cons(Atom, quote("olive"))), cdr(cons(quote("oil"), Atom)))));
        CHECK(IsA(cons(quote("ratatouille"), quote("baguette")),
                  Pair(car(cons(Atom, quote("olive"))), cdr(cons(quote("oil"), Atom)))));
        CHECK(AreTheSameType(Pair(car(cons(Atom, quote("olive"))), cdr(cons(quote("oil"), Atom))), Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, cdr(cons(quote("oil"), Atom))), Pair(Atom, Atom)));
    }

    SECTION("Nat") {
        CHECK(!IsA(quote("6"), Atom));
        CHECK(!IsA(cons(quote("17"), quote("pepper")), Pair(Atom, Atom)));
        CHECK(IsA(nat<1>(), Nat));
        CHECK(IsA(nat<1729>(), Nat));
        CHECK(!IsA(nat<-1>(), Nat));
        CHECK(!IsA(nat<-23>(), Nat));
        CHECK(IsA(nat<0>(), Nat));
        CHECK(!IsTheSameAs(Nat, nat<0>(), nat<26>()));
        CHECK(IsTheSameAs(Nat, plus(nat<0>(), nat<26>()), nat<26>()));
        CHECK(IsTheSameAs(Nat, nat<0>(), zero));
        CHECK(IsTheSameAs(Nat, nat<1>(), one));
        CHECK(IsTheSameAs(Nat, nat<2>(), two));
        CHECK(IsTheSameAs(Nat, nat<4>(), four));
        CHECK(IsTheSameAs(Nat, nat<8>(), eight));
        CHECK(IsANormal(nat<8>(), Nat));
        CHECK(IsANormal(nat<7>(), Nat));
        CHECK(IsANormal(zero, Nat));
        CHECK(IsANormal(add1(zero), Nat));
    }

    SECTION("values") {
        CHECK(!IsANormal(add1(plus(add1(zero), add1(add1(zero)))), Nat));
        CHECK(IsAValue(Nat, add1(plus(add1(zero), add1(add1(zero))))));
        CHECK(!IsANormal(plus(add1(add1(zero)), add1(zero)), Nat));
        CHECK(!IsANormal(add1(plus(add1(zero), add1(one))), Nat));
        CHECK(!IsAValue(Nat, add1(quote("aubergine"))));
        CHECK(IsTheSameAs(Nat, add1(plus(add1(zero), add1(add1(zero)))), four));
        CHECK(IsTheSameAs(Nat, add1(zero), add1(zero)));
        CHECK(IsTheSameAs(Nat, add1(plus(nat<0>(), nat<1>())), add1(plus(nat<1>(), nat<0>()))));
        CHECK(IsTheValueOf(
            Nat, ComputeValue(plus(nat<3>(), nat<5>())), car(cons(plus(nat<3>(), nat<5>()), quote("baguette")))));
        CHECK(!IsA(cons(zero, quote("onion")), Pair(Atom, Atom)));
        CHECK(IsA(cons(zero, quote("onion")), Pair(Nat, Atom)));
        CHECK(IsA(cons(quote("zero"), quote("onion")), Pair(Atom, Atom)));
        CHECK(IsA(cons(quote("basil"), cons(quote("thyme"), quote("oregano"))), Pair(Atom, Pair(Atom, Atom))));
    }
}
