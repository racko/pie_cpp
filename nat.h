#pragma once

#include "pie_base.h"
#include <ostream>
#include <utility>

struct Nat_t : Pie<Nat_t> {};

inline constexpr Nat_t Nat;

constexpr bool equal(Nat_t, Nat_t, int&) { return true; }

inline void print(std::ostream& s, Nat_t, int&) { s << "Nat"; }

struct Zero_t : Pie<Zero_t> {};

inline constexpr Zero_t zero;

constexpr bool equal(Zero_t, Zero_t, int&) { return true; }

inline void print(std::ostream& s, Zero_t, int&) { s << "zero"; }

template <typename Derived>
struct Add1_t : Pie<Add1_t<Derived>> {
    constexpr Add1_t(const Derived& smaller) : smaller_{smaller} {}

    Derived smaller_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal(const Add1_t<Derived1>& lhs, const Add1_t<Derived2>& rhs, int& next_index) {
    return equal(lhs.smaller_, rhs.smaller_, next_index);
}

template <typename Derived>
void print(std::ostream& s, const Add1_t<Derived>& n, int& next_index) {
    s << "(add1 " << InContext(n.smaller_, next_index) << ')';
}

template <typename Derived>
constexpr Add1_t<Derived> add1(const Pie<Derived>& n) {
    return Add1_t<Derived>{n};
}

template <std::int32_t N>
struct Int32 : Pie<Int32<N>> {};

template <std::int32_t N1, std::int32_t N2>
constexpr bool equal(Int32<N1>, Int32<N2>, int&) {
    return N1 == N2;
}

constexpr bool equal(Int32<0>, Zero_t, int&) { return true; }

constexpr bool equal(Zero_t, Int32<0>, int&) { return true; }

template <typename Derived, std::int32_t N>
constexpr bool equal(const Add1_t<Derived>& lhs, Int32<N>, int& next_index) {
    return equal(lhs.smaller_, Int32<N - 1>{}, next_index);
}

template <std::int32_t N, typename Derived>
constexpr bool equal(Int32<N> lhs, const Add1_t<Derived>& rhs, int& next_index) {
    return equal(rhs, lhs, next_index);
}

template <std::int32_t N>
void print(std::ostream& s, Int32<N>, int&) {
    s << N;
}

template <std::int32_t N>
constexpr Int32<N> nat() {
    return Int32<N>{};
}

template <typename Target, typename Base, typename Step>
struct IterNat_t : Pie<IterNat_t<Target, Base, Step>> {
    constexpr IterNat_t(const Target& target, const Base& base, const Step& step) : target_{target}, base_{base}, step_{step} {}

    Target target_;
    Base base_;
    Step step_;
};

template <typename Target1, typename Base1, typename Step1, typename Target2, typename Base2, typename Step2>
constexpr bool equal(const IterNat_t<Target1, Base1, Step1>& lhs, const IterNat_t<Target2, Base2, Step2>& rhs, int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.base_, rhs.base_, next_index) && equal(lhs.step_, rhs.step_, next_index);
}

template <typename Target, typename Base, typename Step>
void print(std::ostream& s, const IterNat_t<Target, Base, Step>& x, int& next_index) {
    s << "(iter-Nat " << InContext(x.target_, next_index) << ' ' << InContext(x.base_, next_index) << ' '
      << InContext(x.step_, next_index) << ')';
}

template <typename Target, typename Base, typename Step>
constexpr IterNat_t<Target, Base, Step>
iter_Nat(const Pie<Target>& target, const Pie<Base>& base, const Pie<Step>& step) {
    return IterNat_t<Target, Base, Step>{target.derived(), base.derived(), step.derived()};
}

constexpr bool IsA1(Zero_t, Nat_t, int&) { return true; }

template <typename Derived>
constexpr bool IsA1(const Add1_t<Derived>& n, Nat_t, int& next_index) {
    return IsA1(n.smaller_, Nat, next_index);
}

template <std::int32_t N>
constexpr bool IsA1(Int32<N>, Nat_t, int&) {
    return N >= 0;
}

constexpr bool IsAType1(Nat_t, int&) { return true; }

template <>
struct is_normal<Nat_t> : std::true_type {};

template <>
struct is_value<Nat_t> : std::true_type {};

template <std::int32_t N>
struct is_normal<Int32<N>> : std::true_type {};

template <std::int32_t N>
struct is_value<Int32<N>> : std::true_type {};

template <>
struct is_normal<Zero_t> : std::true_type {};

template <>
struct is_value<Zero_t> : std::true_type {};

template <typename Derived>
struct is_normal<Add1_t<Derived>> : std::bool_constant<is_normal_v<Derived>> {};

template <typename Derived>
struct is_value<Add1_t<Derived>> : std::true_type {};

template <typename Target, typename Base, typename Step>
struct step_Iter_Nat_result;

template <typename Target, typename Base, typename Step>
using step_Iter_Nat_result_t = typename step_Iter_Nat_result<Target, Base, Step>::type;

template <typename Base, typename Step>
struct step_Iter_Nat_result<Int32<0>, Base, Step> {
    using type = Base;
};

template <typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Int32<0>, Base, Step> Step_Iter_Nat(Int32<0>, const Base& base, const Step&) {
    return base;
}

template <std::int32_t N, typename Base, typename Step>
struct step_Iter_Nat_result<Int32<N>, Base, Step> {
    using type = std::invoke_result_t<Step, IterNat_t<Int32<N - 1>, Base, Step>>;
};

template <std::int32_t N, typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Int32<N>, Base, Step> Step_Iter_Nat(Int32<N>, const Base& base, const Step& step) {
    return step(iter_Nat(Int32<N - 1>{}, base, step));
}

template <typename Base, typename Step>
struct step_Iter_Nat_result<Zero_t, Base, Step> {
    using type = Base;
};

template <typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Zero_t, Base, Step> Step_Iter_Nat(Zero_t, const Base& base, const Step&) {
    return base;
}

template <typename Derived, typename Base, typename Step>
struct step_Iter_Nat_result<Add1_t<Derived>, Base, Step> {
    using type = std::invoke_result_t<Step, IterNat_t<Derived, Base, Step>>;
};

template <typename Derived, typename Base, typename Step>
constexpr step_Iter_Nat_result_t<Add1_t<Derived>, Base, Step>
Step_Iter_Nat(const Add1_t<Derived>& target, const Base& base, const Step& step) {
    return step(iter_Nat(target.smaller_, base, step));
}

template <typename Target, typename Base, typename Step>
struct step_result<IterNat_t<Target, Base, Step>> {
    using type = step_Iter_Nat_result_t<compute_value_result_t<Target>, Base, Step>;
};

template <typename Target, typename Base, typename StepT>
constexpr step_result_t<IterNat_t<Target, Base, StepT>> Step(const IterNat_t<Target, Base, StepT>& iter_nat) {
    assert(!is_neutral_v<Target>);
    return Step_Iter_Nat(ComputeValue(iter_nat.target_), iter_nat.base_, iter_nat.step_);
}

template <typename Target, typename Base, typename Step>
struct is_neutral<IterNat_t<Target, Base, Step>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Derived>
struct normalize_result1<Add1_t<Derived>, false> {
    using type = Add1_t<normalize_result_t<Derived>>;
};

template <typename Derived>
constexpr normalize_result_t<Add1_t<Derived>> Normalize(const Add1_t<Derived>& x, std::false_type /*is_normal*/) {
    return add1(Normalize(x.smaller_));
}

template <>
struct synth_result<Nat_t> {
    using type = U_t;
};

constexpr synth_result_t<Nat_t> synth1(Nat_t, int&) { return U; }

template <std::int32_t N>
struct synth_result<Int32<N>> {
    using type = Nat_t;
};

template <std::int32_t N>
constexpr synth_result_t<Int32<N>> synth1(Int32<N>, int&) {
    return Nat;
}

template <>
struct synth_result<Zero_t> {
    using type = Nat_t;
};

constexpr synth_result_t<Zero_t> synth1(Zero_t, int&) { return Nat; }

template <typename Derived>
struct synth_result<Add1_t<Derived>> {
    using type = Nat_t;
};

template <typename Derived>
constexpr synth_result_t<Add1_t<Derived>> synth1(Add1_t<Derived>, int&) {
    return Nat;
}

template <typename Target, typename Base, typename Step>
struct synth_result<IterNat_t<Target, Base, Step>> {
    using type = synth_result_t<Base>;
};

template <typename Target, typename Base, typename Step>
constexpr synth_result_t<IterNat_t<Target, Base, Step>> synth1(const IterNat_t<Target, Base, Step>& x, int& next_index) {
    const auto base_type = synth1(x.base_, next_index);
    assert(IsA1(x.target_, Nat, next_index) && IsA1(x.step_, Arrow(base_type, base_type), next_index));
    return base_type;
}
