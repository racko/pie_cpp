#include "prelude.h"

#include <catch2/catch.hpp>

// TODO: don't use sections? They are a fixture like mechanism: Catch runs all sections that are on the same level with
// the same environment defined by the surrounding section by restarting after every section ...
//
// TODO: mark each CHECK with frame number and page

struct quote_prune {
    static inline constexpr const char* value = "prune";
};

struct quote_atom {
    static inline constexpr const char* value = "atom";
};

struct quote_ratatouille {
    static inline constexpr const char* value = "ratatouille";
};

struct quote_is_it_obvious_that_this_is_an_atom {
    static inline constexpr const char* value = "is-it-obvious-that-this-is-an-atom";
};

struct quote_four_dashes {
    static inline constexpr const char* value = "----";
};

struct quote_three_dashes {
    static inline constexpr const char* value = "---";
};

struct empty_quote {
    static inline constexpr const char* value = "";
};

struct quote_Atom {
    static inline constexpr const char* value = "Atom";
};

struct quote_at0m {
    static inline constexpr const char* value = "at0m";
};

struct quote_courgette {
    static inline constexpr const char* value = "courgette";
};

struct quote_baguette {
    static inline constexpr const char* value = "baguette";
};

struct quote_citron {
    static inline constexpr const char* value = "citron";
};

struct quote_pomme {
    static inline constexpr const char* value = "pomme";
};

struct quote_orange {
    static inline constexpr const char* value = "orange";
};

struct quote_aubergine {
    static inline constexpr const char* value = "aubergine";
};

struct quote_tomato {
    static inline constexpr const char* value = "tomato";
};

struct quote_olive_oil {
    static inline constexpr const char* value = "olive-oil";
};

struct quote_olive {
    static inline constexpr const char* value = "olive";
};

struct quote_oil {
    static inline constexpr const char* value = "oil";
};

struct quote_6 {
    static inline constexpr const char* value = "6";
};

struct quote_17 {
    static inline constexpr const char* value = "17";
};

struct quote_pepper {
    static inline constexpr const char* value = "pepper";
};

struct quote_onion {
    static inline constexpr const char* value = "onion";
};

struct quote_zero {
    static inline constexpr const char* value = "zero";
};

struct quote_basil {
    static inline constexpr const char* value = "basil";
};

struct quote_thyme {
    static inline constexpr const char* value = "thyme";
};

struct quote_oregano {
    static inline constexpr const char* value = "oregano";
};

TEST_CASE("Chapter1", "") {
    SECTION("IsA Atom") {
        CHECK(IsA(quote<quote_atom>(), Atom));
        CHECK(IsA(quote<quote_ratatouille>(), Atom));
        CHECK(IsA(quote<quote_is_it_obvious_that_this_is_an_atom>(), Atom));
        CHECK(IsA(quote<quote_four_dashes>(), Atom));
        CHECK(IsA(quote<quote_three_dashes>(), Atom));
        // CHECK(!IsA("---", Atom));
        CHECK(!IsA(quote<empty_quote>(), Atom));
        CHECK(IsA(quote<quote_Atom>(), Atom));
        CHECK(!IsA(quote<quote_at0m>(), Atom));
        // CHECK(IsA(quote<quote_cœur-d-artichauts>(), Atom));
        // CHECK(IsA(quote<quote_ἄτομον>(), Atom));
        CHECK(IsA(quote<quote_courgette>(), Atom));
    }

    SECTION("IsTheSameAs Atom") {
        CHECK(IsTheSameAs(Atom, quote<quote_ratatouille>(), quote<quote_ratatouille>()));
        CHECK(!IsTheSameAs(Atom, quote<quote_ratatouille>(), quote<quote_baguette>()));
        CHECK(IsTheSameAs(Atom, quote<quote_citron>(), quote<quote_citron>()));
        CHECK(!IsTheSameAs(Atom, quote<quote_pomme>(), quote<quote_orange>()));
    }

    SECTION("IsA Pair") { CHECK(IsA(cons(quote<quote_ratatouille>(), quote<quote_baguette>()), Pair(Atom, Atom))); }

    SECTION("IsTheSameAs Pair") {
        CHECK(IsTheSameAs(Pair(Atom, Atom),
                          cons(quote<quote_ratatouille>(), quote<quote_baguette>()),
                          cons(quote<quote_ratatouille>(), quote<quote_baguette>())));
        CHECK(!IsTheSameAs(Pair(Atom, Atom),
                           cons(quote<quote_ratatouille>(), quote<quote_baguette>()),
                           cons(quote<quote_baguette>(), quote<quote_baguette>())));
    }

    SECTION("IsAType") {
        CHECK(IsAType(Atom));
        CHECK(IsAType(Pair(Atom, Atom)));
        CHECK(!IsAType(quote<quote_courgette>()));
    }

    SECTION("AreTheSameType") {
        CHECK(AreTheSameType(Atom, Atom));
        CHECK(!AreTheSameType(Atom, Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, Atom), Pair(Atom, Atom)));
    }

    // TODO: This asserts. How to test for that?
    // SECTION("Presuppositions") { CHECK(!IsTheSameAs(quote<quote_fruit>(), quote<quote_pêche>(),
    // quote<quote_pêche>())); }

    SECTION("car and cdr") {
        CHECK(IsTheSameAs(Atom,
                          car(the(Pair(Atom, Atom), cons(quote<quote_ratatouille>(), quote<quote_baguette>()))),
                          quote<quote_ratatouille>()));
        CHECK(IsTheSameAs(Atom,
                          cdr(the(Pair(Atom, Atom), cons(quote<quote_ratatouille>(), quote<quote_baguette>()))),
                          quote<quote_baguette>()));
        CHECK(IsA(car(the(Pair(Pair(Atom, Atom), Atom),
                          cons(cons(quote<quote_aubergine>(), quote<quote_courgette>()), quote<quote_tomato>()))),
                  Pair(Atom, Atom)));
        CHECK(IsTheSameAs(
            Atom,
            car(cdr(the(Pair(Atom, Pair(Atom, Atom)),
                        cons(quote<quote_ratatouille>(), cons(quote<quote_baguette>(), quote<quote_olive_oil>()))))),
            quote<quote_baguette>()));
    }

    SECTION("normal forms") {
        CHECK(IsNormalFormOf(
            Atom,
            quote<quote_olive_oil>(),
            cdr(cdr(cons(quote<quote_ratatouille>(), cons(quote<quote_baguette>(), quote<quote_olive_oil>()))))));
        CHECK(IsANormal(cons(quote<quote_ratatouille>(), quote<quote_baguette>()), Pair(Atom, Atom)));
        CHECK(
            IsNormalFormOf(Pair(Atom, Atom),
                           cons(quote<quote_aubergine>(), quote<quote_courgette>()),
                           car(cons(cons(quote<quote_aubergine>(), quote<quote_courgette>()), quote<quote_tomato>()))));
        CHECK(
            IsTheSameAs(Pair(Atom, Atom),
                        car(the(Pair(Pair(Atom, Atom), Atom),
                                cons(cons(quote<quote_aubergine>(), quote<quote_courgette>()), quote<quote_tomato>()))),
                        cons(quote<quote_aubergine>(), quote<quote_courgette>())));
        CHECK(IsTheSameAs(
            Pair(Atom, Atom),
            car(the(Pair(Pair(Atom, Atom), Atom),
                    cons(cons(quote<quote_aubergine>(), quote<quote_courgette>()), quote<quote_tomato>()))),
            car(the(Pair(Pair(Atom, Atom), Atom),
                    cons(cons(quote<quote_aubergine>(), quote<quote_courgette>()), quote<quote_tomato>())))));
        // CHECK(IsIllTyped(Pair(cdr(cons(Atom, quote<quote_olive>())), car(cons(quote<quote_oil>(), Atom)))));
        CHECK(IsNormalFormOfType(Normalize(Pair(Atom, Atom)),
                                 Pair(car(cons(Atom, quote<quote_olive>())), cdr(cons(quote<quote_oil>(), Atom)))));
        CHECK(IsA(cons(quote<quote_ratatouille>(), quote<quote_baguette>()),
                  Pair(car(cons(Atom, quote<quote_olive>())), cdr(cons(quote<quote_oil>(), Atom)))));
        CHECK(AreTheSameType(Pair(car(cons(Atom, quote<quote_olive>())), cdr(cons(quote<quote_oil>(), Atom))),
                             Pair(Atom, Atom)));
        CHECK(AreTheSameType(Pair(Atom, cdr(cons(quote<quote_oil>(), Atom))), Pair(Atom, Atom)));
    }

    SECTION("Nat") {
        CHECK(!IsA(quote<quote_6>(), Atom));
        CHECK(!IsA(cons(quote<quote_17>(), quote<quote_pepper>()), Pair(Atom, Atom)));
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
        CHECK(!IsAValue(Nat, add1(quote<quote_aubergine>())));
        CHECK(IsTheSameAs(Nat, add1(plus(add1(zero), add1(add1(zero)))), four));
        CHECK(IsTheSameAs(Nat, add1(zero), add1(zero)));
        CHECK(IsTheSameAs(Nat, add1(plus(nat<0>(), nat<1>())), add1(plus(nat<1>(), nat<0>()))));
        CHECK(IsTheValueOf(
            Nat, ComputeValue(plus(nat<3>(), nat<5>())), car(cons(plus(nat<3>(), nat<5>()), quote<quote_baguette>()))));
        CHECK(!IsA(cons(zero, quote<quote_onion>()), Pair(Atom, Atom)));
        CHECK(IsA(cons(zero, quote<quote_onion>()), Pair(Nat, Atom)));
        CHECK(IsA(cons(quote<quote_zero>(), quote<quote_onion>()), Pair(Atom, Atom)));
        CHECK(IsA(cons(quote<quote_basil>(), cons(quote<quote_thyme>(), quote<quote_oregano>())),
                  Pair(Atom, Pair(Atom, Atom))));
    }
}
