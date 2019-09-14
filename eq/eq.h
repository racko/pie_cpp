#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Derived1, typename Derived2, typename Derived3>
struct Eq_t : Pie<Eq_t<Derived1, Derived2, Derived3>> {
    using type = Derived1;
    using from_t = Derived2;
    using to_t = Derived3;

    constexpr Eq_t(Derived1 type, Derived2 from, Derived3 to)
        : type_{std::move(type)}, from_{std::move(from)}, to_{std::move(to)} {}

    Derived1 type_;
    Derived2 from_;
    Derived3 to_;
};

template <typename Derived1,
          typename Derived2,
          typename Derived3,
          typename Derived4,
          typename Derived5,
          typename Derived6>
constexpr bool
equal(const Eq_t<Derived1, Derived2, Derived3>& lhs, const Eq_t<Derived4, Derived5, Derived6>& rhs, int& next_index) {
    return equal(lhs.type_, rhs.type_, next_index) && equal(lhs.from_, rhs.from_, next_index) &&
           equal(lhs.to_, rhs.to_, next_index);
}

template <typename Derived1, typename Derived2, typename Derived3>
void print(std::ostream& s, const Eq_t<Derived1, Derived2, Derived3>& eq, int& next_index) {
    s << "(= " << InContext(eq.type_, next_index) << ' ' << InContext(eq.from_, next_index) << ' '
      << InContext(eq.to_, next_index) << ')';
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr Eq_t<Derived1, Derived2, Derived3>
Eq(const Pie<Derived1>& type, const Pie<Derived2>& from, const Pie<Derived3>& to) {
    return Eq_t<Derived1, Derived2, Derived3>(type.derived(), from.derived(), to.derived());
}

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
constexpr bool IsAType1(const Eq_t<DerivedT, DerivedFrom, DerivedTo>& eq, int& next_index) {
    return IsAType1(eq.type_, next_index) && IsA1(eq.from_, eq.type_, next_index) && IsA1(eq.to_, eq.type_, next_index);
}

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
struct is_normal<Eq_t<DerivedT, DerivedFrom, DerivedTo>>
    : std::bool_constant<is_normal_v<DerivedT> && is_normal_v<DerivedFrom> && is_normal_v<DerivedTo>> {};

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
struct is_value<Eq_t<DerivedT, DerivedFrom, DerivedTo>> : std::true_type {};

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
struct normalize_result1<Eq_t<DerivedT, DerivedFrom, DerivedTo>, false> {
    using type = Eq_t<normalize_result_t<DerivedT>, normalize_result_t<DerivedFrom>, normalize_result_t<DerivedTo>>;
};

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
constexpr normalize_result_t<Eq_t<DerivedT, DerivedFrom, DerivedTo>>
Normalize(const Eq_t<DerivedT, DerivedFrom, DerivedTo>& type, std::false_type /*is_normal*/) {
    return Eq_t(Normalize(type.type_), Normalize(type.from_), Normalize(type.to_));
}

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
struct synth_result<Eq_t<DerivedT, DerivedFrom, DerivedTo>> {
    using type = U_t;
};

template <typename DerivedT, typename DerivedFrom, typename DerivedTo>
constexpr synth_result_t<Eq_t<DerivedT, DerivedFrom, DerivedTo>> synth1(Eq_t<DerivedT, DerivedFrom, DerivedTo>, int&) {
    return U;
}
