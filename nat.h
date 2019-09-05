#pragma once

#include "pie_base.h"
#include <utility>
#include <ostream>

struct Nat_t : Pie<Nat_t> {};

inline constexpr Nat_t Nat;

bool operator==(Nat_t, Nat_t) {
    return true;
}

std::ostream& operator<<(std::ostream& s, Nat_t) {
    return s << "Nat";
}

struct Zero_t : Pie<Zero_t> {};

inline constexpr Zero_t zero;

bool operator==(Zero_t, Zero_t) {
    return true;
}

std::ostream& operator<<(std::ostream& s, Zero_t) {
    return s << "zero";
}

template <typename Derived>
struct Add1_t : Pie<Add1_t<Derived>> {
    constexpr Add1_t(const Derived& smaller) : smaller_{smaller} {}

    Derived smaller_;
};

template <typename Derived1, typename Derived2>
bool operator==(const Add1_t<Derived1>&, const Add1_t<Derived2>&) {
    return false;
}

template <typename Derived>
bool operator==(const Add1_t<Derived>& lhs, const Add1_t<Derived>& rhs) {
    return lhs.smaller_ == rhs.smaller_;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const Add1_t<Derived>& n) {
    return s << "(add1 " << n.smaller_ << ')';
}

template <typename Derived>
constexpr Add1_t<Derived> add1(const Pie<Derived>& n) {
    return Add1_t<Derived>{n};
}

inline constexpr auto one{add1(zero)};
inline constexpr auto two{add1(one)};
inline constexpr auto three{add1(two)};
inline constexpr auto four{add1(three)};
inline constexpr auto five{add1(four)};
inline constexpr auto six{add1(five)};
inline constexpr auto seven{add1(six)};
inline constexpr auto eight{add1(seven)};

struct DynamicNat : Pie<DynamicNat> {
    DynamicNat(int n) : n_{n} {}

    int n_;
};

bool operator==(const DynamicNat lhs, const DynamicNat rhs) {
    return lhs.n_ == rhs.n_;
}

bool operator==(const DynamicNat lhs, Zero_t) {
    return lhs.n_ == 0;
}

bool operator==(Zero_t, const DynamicNat rhs) {
    return 0 == rhs.n_;
}

template <typename Derived>
bool operator==(const Add1_t<Derived>& lhs, const DynamicNat rhs) {
    return lhs.smaller_ == DynamicNat{rhs.n_ - 1};
}

template <typename Derived>
bool operator==(const DynamicNat lhs, const Add1_t<Derived>& rhs) {
    return rhs == lhs;
}

std::ostream& operator<<(std::ostream& s, const DynamicNat n) {
    return s << n.n_;
}

DynamicNat nat(const int n) { return DynamicNat(n); }

template <typename Derived1, typename Derived2>
struct Plus_t : Pie<Plus_t<Derived1, Derived2>> {
    Plus_t(Derived1 lhs, Derived2 rhs) : lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

    Derived1 lhs_;
    Derived2 rhs_;
};

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
bool operator==(const Plus_t<Derived1, Derived2>&, const Plus_t<Derived3, Derived4>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool operator==(const Plus_t<Derived1, Derived2>& lhs, const Plus_t<Derived1, Derived2>& rhs) {
    return lhs.lhs_ == rhs.lhs_ && lhs.rhs_ == rhs.rhs_;
}

template <typename Derived1, typename Derived2>
std::ostream& operator<<(std::ostream& s, const Plus_t<Derived1, Derived2>& sum) {
    return s << "(plus " << sum.lhs_ << ' ' << sum.rhs_ << ')';
}

template <typename Derived1, typename Derived2>
Plus_t<Derived1, Derived2> plus(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    return Plus_t<Derived1, Derived2>(lhs, rhs);
}

bool IsA_For_Values(Zero_t, Nat_t) { return true; }

template <typename Derived>
bool IsA_For_Values(const Add1_t<Derived>& n, Nat_t) {
    return IsA(n.smaller_, Nat);
}

bool IsA_For_Values(const DynamicNat n, Nat_t) { return n.n_ >= 0; }

bool IsTheSameAs_For_Values(Nat_t, Zero_t, Zero_t) { return true; }

template <typename Derived1, typename Derived2>
bool IsTheSameAs_For_Values(Nat_t, const Add1_t<Derived1>& lhs, const Add1_t<Derived2>& rhs) {
    return IsTheSameAs(Nat, lhs.smaller_, rhs.smaller_);
}

bool IsTheSameAs_For_Values(Nat_t, Zero_t, const DynamicNat n) { return n.n_ == 0; }
bool IsTheSameAs_For_Values(Nat_t, const DynamicNat n, Zero_t) { return IsTheSameAs(Nat, zero, n); }

template <typename Derived>
bool IsTheSameAs_For_Values(Nat_t, const Add1_t<Derived>& lhs, const DynamicNat rhs) {
    return IsTheSameAs(Nat, lhs.smaller_, DynamicNat{rhs.n_ - 1});
}

template <typename Derived>
bool IsTheSameAs_For_Values(Nat_t, const DynamicNat lhs, const Add1_t<Derived>& rhs) {
    return IsTheSameAs(Nat, rhs, lhs);
}

bool IsTheSameAs_For_Values(Nat_t, const DynamicNat lhs, const DynamicNat rhs) { return lhs.n_ == rhs.n_; }

bool IsAType_For_Values(Nat_t) { return true; }

bool AreTheSameType_For_Values(Nat_t, Nat_t) { return true; }

bool IsANormal(Zero_t, Nat_t) { return true; }

template <typename Derived>
bool IsANormal(const Add1_t<Derived>& n, Nat_t) {
    return IsANormal(n.smaller_, Nat);
}

bool IsANormal(const DynamicNat n, Nat_t) { return IsA(n, Nat); }

bool IsANormalType(Nat_t) {
    return true;
}

bool IsNormalFormOfType(Nat_t, Nat_t) { return true; }

bool IsAValue(Nat_t, Zero_t) { return true; }

template <typename Derived>
bool IsAValue(Nat_t, const Add1_t<Derived>& n) {
    return IsA(n.smaller_, Nat);
}

bool IsAValue(Nat_t, const DynamicNat n) { return IsA(n, Nat); }

Nat_t ComputeValue(Nat_t) { return Nat; }

Zero_t ComputeValue(Zero_t) { return zero; }

DynamicNat ComputeValue(const DynamicNat n) { return n; }

template <typename Derived>
Add1_t<Derived> ComputeValue(const Add1_t<Derived>& n) {
    return n;
}

DynamicNat plus_helper(const DynamicNat lhs, const DynamicNat rhs) { return DynamicNat(lhs.n_ + rhs.n_); }

template<typename Derived>
Derived plus_helper(Zero_t, const Pie<Derived>& rhs) { return rhs; }

template <typename Derived1, typename Derived2>
Add1_t<Plus_t<Derived1, Derived2>> plus_helper(const Add1_t<Derived1>& lhs, const Pie<Derived2> rhs) {
    return add1(plus(lhs.smaller_, rhs));
}

template <typename Derived1, typename Derived2>
auto plus_helper(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    return plus_helper(rhs, lhs);
}

template <typename Derived1, typename Derived2>
auto ComputeValue(const Plus_t<Derived1, Derived2>& sum) {
    return plus_helper(ComputeValue(sum.lhs_), ComputeValue(sum.rhs_));
}

Nat_t Normalize(Nat_t) { return Nat; }

Zero_t Normalize(Zero_t) { return zero; }

DynamicNat Normalize(const DynamicNat n) { return n; }

template <typename Derived>
Add1_t<Derived> Normalize(const Add1_t<Derived>& n) {
    return n;
}

template <typename Derived1, typename Derived2>
auto Normalize(const Plus_t<Derived1, Derived2>& sum) {
    return Normalize(ComputeValue(sum)); // No clue if this makes sense ...
}
