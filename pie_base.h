#pragma once

template <typename Derived>
struct Pie {
    constexpr operator Derived&() { return static_cast<Derived&>(*this); }
    constexpr operator Derived const&() const { return static_cast<const Derived&>(*this); }

    constexpr Derived& derived() { return static_cast<Derived&>(*this); }
    constexpr Derived const& derived() const { return static_cast<const Derived&>(*this); }
};

struct Universe_t : Pie<Universe_t> {};

inline constexpr Universe_t Universe;

//template <typename Derived1, typename Derived2>
//bool IsA_For_Values(const Pie<Derived1>& value, const Pie<Derived2>& type) {
//    return IsAType_For_Values(type.derived()) && IsA_For_Values(value.derived(), type.derived());
//}

template <typename Derived1, typename Derived2>
bool IsA_For_Values(const Pie<Derived1>&, const Pie<Derived2>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool IsA(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    return IsA_For_Values(ComputeValue(value.derived()), ComputeValue(type.derived()));
    // return IsAType(type.derived()) && IsA(ComputeValue(value.derived()), ComputeValue(type.derived()));
}

//template <typename Derived1, typename Derived2, typename Derived3>
//bool IsTheSameAs_For_Values(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
//    return IsAType(type.derived()) && IsA(lhs.derived(), type.derived()) && IsA(rhs.derived(), type.derived()) &&
//           IsTheSameAs_For_Values(type.derived(), lhs.derived(), rhs.derived());
//}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsTheSameAs_For_Values(const Pie<Derived1>&, const Pie<Derived2>&, const Pie<Derived3>&) {
    return false;
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsTheSameAs(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    return IsTheSameAs_For_Values(
        ComputeValue(type.derived()), ComputeValue(lhs.derived()), ComputeValue(rhs.derived()));
    // return IsAType(type.derived()) && IsA(lhs.derived(), type.derived()) && IsA(rhs.derived(), type.derived()) &&
    // IsTheSameAs(ComputeValue(type.derived()), ComputeValue(lhs.derived()), ComputeValue(rhs.derived()));
}

//template <typename Derived>
//bool IsAType_For_Values(const Pie<Derived>& type) {
//    //return IsAValue(Universe, type.derived()) ? false : IsAType(type.derived());
//    return IsAType(type.derived());
//}

template <typename Derived>
bool IsAType_For_Values(const Pie<Derived>&) {
    return false;
}

template <typename Derived>
bool IsAType(const Pie<Derived>& type) {
    return IsAType_For_Values(ComputeValue(type.derived()));
    //// If it is a value but other overloads didn't match, we know that it isn't a type.
    //return IsAValue(Universe, type.derived()) ? false : IsAType(ComputeValue(type.derived()));
}

// TODO IsA / IsAValue for Universe. Requires synth because it checks whether the (synthesized) type of "type"
// IsTheSameAs Universe

template <typename Derived1, typename Derived2>
bool AreTheSameType_For_Values(const Pie<Derived1>&, const Pie<Derived2>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool AreTheSameType(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    return AreTheSameType_For_Values(ComputeValue(lhs.derived()), ComputeValue(rhs.derived()));
}

template <typename Derived1, typename Derived2>
bool IsANormal(const Pie<Derived1>&, const Pie<Derived2>&) {
    return false;
}

template <typename Derived1, typename Derived2>
bool IsNormalFormOfType(const Pie<Derived1>& type, const Pie<Derived2>& nf) {
    // TODO Replace with one of
    // - IsANormalType(nf)
    // - IsANormal(nf, Universe)
    // Can't do: AreTheSameType(nf, normalize(nf)) because AreTheSameType normalizes
    return AreTheSameType(type.derived(), nf.derived()) && true;
}

template <typename Derived1, typename Derived2, typename Derived3>
bool IsNormalFormOf(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    return IsTheSameAs(type.derived(), lhs.derived(), rhs.derived()) && IsANormal(lhs.derived(), type.derived());
}

template <typename Derived1, typename Derived2>
bool IsAValue(const Pie<Derived1>&, const Pie<Derived2>&) {
    return false;
}

template <typename Derived>
bool IsIllTyped(const Pie<Derived>&) {
    return false; // TODO not (type or expression described by a type). For this we need synth
}

// template <typename Derived>
// auto ComputeValue(const Pie<Derived>& value);
