#pragma once

#include <cassert>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>

//#define DEBUG

#ifdef DEBUG
#define LOG(msg)                                                                                                       \
    do {                                                                                                               \
        std::cerr << msg << '\n';                                                                                      \
    } while (0)
#define CONSTEXPR const
#define CONSTEXPR_FUNC inline
#else
#define LOG(msg)                                                                                                       \
    do {                                                                                                               \
    } while (0)
#define CONSTEXPR constexpr
#define CONSTEXPR_FUNC constexpr
#endif

// SORRY :(
template <typename F, typename Arg>
struct App_t;

template <typename F, typename... Args>
struct apply_result;

template <typename F, typename... Args>
using apply_result_t = typename apply_result<F, Args...>::type;

template <typename F>
struct apply_result<F> {
    using type = F;
};

template <typename F, typename Arg, typename... Args>
struct apply_result<F, Arg, Args...> {
    using type = apply_result_t<App_t<F, Arg>, Args...>;
};

template <typename Derived>
struct Pie {
    template <typename... Args>
    constexpr apply_result_t<Derived, Args...> operator()(const Pie<Args>&... args) const;

    constexpr operator Derived&() { return static_cast<Derived&>(*this); }
    constexpr operator Derived const&() const { return static_cast<const Derived&>(*this); }

    constexpr Derived& derived() { return static_cast<Derived&>(*this); }
    constexpr Derived const& derived() const { return static_cast<const Derived&>(*this); }
};

template <typename T1, typename T2>
struct Equal : std::false_type {};

template <typename T1, typename T2>
constexpr inline bool equal_v = Equal<T1, T2>::value;

template <typename Derived1, typename Derived2>
constexpr bool operator==([[maybe_unused]] const Pie<Derived1>& lhs, [[maybe_unused]] const Pie<Derived2>& rhs) {
    LOG("operator==(" << lhs.derived() << ", " << rhs.derived() << ") ...");
    const bool result = equal_v<Derived1, Derived2>;
    LOG("operator==(" << lhs.derived() << ", " << rhs.derived() << "): " << result);
    return result;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const Pie<Derived>& expr) {
    print(s, expr.derived());
    return s;
}

template <typename T>
struct Height;

template <typename T>
constexpr inline int height_v = Height<T>::value;

template <typename T>
struct is_neutral : std::false_type {};

template <typename T>
inline constexpr bool is_neutral_v = is_neutral<T>::value;

template <typename Derived>
constexpr bool IsNeutral(const Pie<Derived>&) {
    return is_neutral_v<Derived>;
}

template <typename T>
struct is_normal : std::bool_constant<is_neutral_v<T>> {};

template <typename T>
inline constexpr bool is_normal_v = is_normal<T>::value;

template <typename Derived>
constexpr bool IsNormal(const Pie<Derived>&) {
    return is_normal_v<Derived>;
}

template <typename T>
struct is_value : std::bool_constant<is_normal_v<T>> {};

template <typename T>
inline constexpr bool is_value_v = is_value<T>::value;

template <typename Derived>
constexpr bool IsValue(const Pie<Derived>&) {
    return is_value_v<Derived>;
}

template <typename T>
struct step_result;

template <typename T>
using step_result_t = typename step_result<T>::type;

template <typename Derived>
constexpr step_result_t<Derived> Step(const Pie<Derived>& x) {
    LOG("Step(" << x.derived() << ") ...");
    const auto result = Step1(x.derived());
    LOG("Step(" << x.derived() << "): " << result);
    return result;
}

template <typename T>
struct compute_value_result;

template <typename T, bool IsValue>
struct compute_value_result1;

template <typename T>
struct compute_value_result1<T, true> {
    using type = T;
};

template <typename T>
struct compute_value_result1<T, false> {
    using type = typename compute_value_result<step_result_t<T>>::type;
};

template <typename T>
struct compute_value_result {
    using type = typename compute_value_result1<T, is_value_v<T>>::type;
};

template <typename T>
using compute_value_result_t = typename compute_value_result<T>::type;

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::true_type) {
    const auto result = x.derived();
    LOG("ComputeValue(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::false_type) {
    LOG("ComputeValue(" << x.derived() << ") ...");
    const auto result = ComputeValue(Step(x.derived()));
    LOG("ComputeValue(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x) {
    return ComputeValue(x.derived(), is_value<Derived>{});
}

template <typename T>
struct normalize_result;

template <typename T, bool IsNormal>
struct normalize_result1;

template <typename T>
struct normalize_result1<T, true> {
    using type = T;
};

template <typename T>
struct normalize_result1<T, false> {
    using type = typename normalize_result<step_result_t<T>>::type;
};

template <typename T>
struct normalize_result {
    using type = typename normalize_result1<T, is_normal_v<T>>::type;
};

template <typename T>
using normalize_result_t = typename normalize_result<T>::type;

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::true_type /*is_normal*/) {
    const auto result = x.derived();
    LOG("Normalize(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::false_type /*is_normal*/) {
    LOG("Normalize(" << x.derived() << ") ...");
    const auto result = Normalize(Step(x.derived()));
    LOG("Normalize(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x) {
    return Normalize(x.derived(), is_normal<Derived>{});
}

template <typename F>
struct NormalizedLambda {
    constexpr NormalizedLambda(const F& f) : f_{f} {}

    template <typename Arg>
    constexpr normalize_result_t<std::invoke_result_t<F, Arg>> operator()(const Arg& arg) const {
        return Normalize(f_(arg));
    }

    F f_;
};

template <typename T, typename Enabled = void>
struct synth_result;

template <typename T>
using synth_result_t = typename synth_result<T>::type;

template <typename Derived>
constexpr synth_result_t<Derived> synth(const Pie<Derived>& x) {
    LOG("synth(" << x.derived() << ") ...");
    const auto result = synth1(x.derived());
    LOG("synth(" << x.derived() << "): " << result);
    return result;
}

template <typename T, typename = std::void_t<>>
struct can_synth : std::false_type {};

template <typename T>
constexpr inline bool can_synth_v = can_synth<T>::value;

template <typename T>
struct can_synth<T, std::void_t<decltype(synth(std::declval<T>()))>> : std::true_type {};

template <typename Derived1, typename Derived2>
constexpr bool
IsA1(const Pie<Derived1>& value, const Pie<Derived2>& type, std::enable_if_t<can_synth_v<Derived1>, int> = 0) {
    LOG("IsA(" << value.derived() << ", " << type.derived() << ") [default] ...");
    const bool result = AreTheSameType(type.derived(), synth(value.derived()));
    LOG("IsA(" << value.derived() << ", " << type.derived() << ") [default]: " << result);
    return result;
}

template <typename Derived1, typename Derived2>
constexpr bool IsA1([[maybe_unused]] const Pie<Derived1>& value,
                    [[maybe_unused]] const Pie<Derived2>& type,
                    std::enable_if_t<!can_synth_v<Derived1>, int> = 0) {
    LOG("IsA(" << value.derived() << ", " << type.derived() << "): false [can not synth]");
    return false;
}

template <typename Derived1, typename Derived2>
constexpr bool IsA(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    LOG("IsA(" << value.derived() << ", " << type.derived() << ") ...");
    assert(IsAType(type.derived()));
    const bool result = IsA1(value.derived(), ComputeValue(type.derived()));
    LOG("IsA(" << value.derived() << ", " << type.derived() << "): " << result);
    return result;
}

template <typename Derived1, typename Derived2>
constexpr bool IsANormal(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    return IsA(value.derived(), type.derived()) && is_normal_v<Derived1>;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheSameAs(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    LOG("IsTheSameAs(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    assert(IsAType(type.derived()));
    assert(IsA(lhs.derived(), type.derived()));
    assert(IsA(rhs.derived(), type.derived()));
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    const auto normalforms_identical = lhs_norm == rhs_norm;
    LOG("IsTheSameAs(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived()
                       << "): " << normalforms_identical);
    return normalforms_identical;
}

template <typename Derived1, typename Derived2>
constexpr bool IsAValue(const Pie<Derived1>& type, const Pie<Derived2>& expr) {
    return IsA(expr.derived(), type.derived()) && is_value_v<Derived2>;
}

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    LOG("AreTheSameType(" << lhs.derived() << ", " << rhs.derived() << ") ...");
    assert(IsAType(lhs.derived()));
    assert(IsAType(rhs.derived()));
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    const auto normalforms_identical = lhs_norm == rhs_norm;
    LOG("AreTheSameType(" << lhs.derived() << ", " << rhs.derived() << "): " << normalforms_identical);
    return normalforms_identical;
}

template <typename Derived1, typename Derived2>
constexpr bool IsNormalFormOfType(const Pie<Derived1>& nf, const Pie<Derived2>& type) {
    LOG("IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << ") ...");
    const auto actual = Normalize(type.derived());
    const auto identical = actual == nf.derived();
    LOG("IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << "):" << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsNormalFormOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    LOG("IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto actual = Normalize(rhs.derived());
    const auto identical = actual == lhs.derived();
    LOG("IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << "): " << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsTheValueOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    LOG("IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto rhs_value = ComputeValue(rhs.derived());
    const auto identical = rhs_value == lhs.derived();
    LOG("IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << "): " << identical);
    return identical;
}

template <typename Type, typename Expr>
struct Definition : Pie<Definition<Type, Expr>> {
    constexpr Definition(const char* name, const Type& type, const Expr& expr)
        : name_{name}, type_{type}, expr_{expr} {}

    const char* name_;
    Type type_;
    Expr expr_;
};

template <typename Derived1, typename Derived2>
struct Height<Definition<Derived1, Derived2>>
    : std::integral_constant<int, std::max(height_v<Derived1>, height_v<Derived2>)> {};

template <typename Type1, typename Expr1, typename Type2, typename Expr2>
struct Equal<Definition<Type1, Expr1>, Definition<Type2, Expr2>> : std::bool_constant<equal_v<Expr1, Expr2>> {};

template <typename Type, typename Expr, typename T>
struct Equal<Definition<Type, Expr>, T> : std::bool_constant<equal_v<Expr, T>> {};

template <typename T, typename Type, typename Expr>
struct Equal<T, Definition<Type, Expr>> : std::bool_constant<equal_v<T, Expr>> {};

template <typename Type, typename Expr>
void print(std::ostream& s, const Definition<Type, Expr>& def) {
    s << def.name_;
}

template <typename Type, typename Expr>
constexpr Definition<Type, Expr> define(const char* name, const Type& type, const Expr& expr) {
    LOG("checking define " << name << " ...");
    assert(IsA(expr.derived(), type.derived()));
    LOG("checking define " << name << " done");
    return Definition(name, type, expr);
}

template <typename Type, typename Expr>
struct step_result<Definition<Type, Expr>> {
    using type = Expr;
};

template <typename Type, typename Expr>
constexpr step_result_t<Definition<Type, Expr>> Step1(const Definition<Type, Expr>& x) {
    return x.expr_;
}

template <typename Type, typename Expr>
struct synth_result<Definition<Type, Expr>> {
    using type = Type;
};

template <typename Type, typename Expr>
constexpr synth_result_t<Definition<Type, Expr>> synth1(const Definition<Type, Expr>& x) {
    LOG("synth(" << x << "): " << x.type_);
    return x.type_;
}

struct U_t : Pie<U_t> {};

template <>
struct Height<U_t> : std::integral_constant<int, 0> {};

template <>
struct Equal<U_t, U_t> : std::true_type {};

inline void print(std::ostream& s, U_t) { s << 'U'; }

inline constexpr U_t U;

template <>
struct is_normal<U_t> : std::true_type {};

template <>
struct is_value<U_t> : std::true_type {};

template <typename T>
struct is_type;

template <typename T>
constexpr inline bool is_type_v = is_type<T>::value;

template <>
struct is_type<U_t> : std::true_type {};

CONSTEXPR_FUNC bool IsAType1(U_t) {
    LOG("IsAType(U): true");
    return true;
}

template <typename Derived>
constexpr bool IsA(U_t, [[maybe_unused]] const Pie<Derived>& type) {
    LOG("IsA(U, " << type.derived() << "): false (U does not have a type)");
    return false;
}

template <typename Derived>
constexpr bool IsAType1(const Pie<Derived>& type) {
    LOG("IsAType(" << type.derived() << "): ...");
    const bool result = IsA(type.derived(), U);
    LOG("IsAType(" << type.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr bool IsAType(const Pie<Derived>& type) {
    LOG("IsAType(" << type.derived() << "): ...");
    const bool result = IsAType1(ComputeValue(type.derived()));
    LOG("IsAType(" << type.derived() << "): " << result);
    return result;
}
