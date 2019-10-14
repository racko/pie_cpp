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

template <typename Derived>
struct Pie {
    template <typename... Args>
    constexpr auto operator()(const Pie<Args>&... args) const;

    constexpr operator Derived&() { return derived(); }
    constexpr operator Derived const&() const { return derived(); }

    constexpr Derived& derived() { return static_cast<Derived&>(*this); }
    constexpr Derived const& derived() const { return static_cast<const Derived&>(*this); }

  protected:
    ~Pie() = default;
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

template <typename T>
struct Printer;

template <typename T>
struct Print {};

template <typename Derived>
std::ostream& operator<<(std::ostream& s, Print<Derived>) {
    Printer<Derived>::print(s);
    return s;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, Pie<Derived>) {
    return s << Print<Derived>{};
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
struct StepResult;

template <typename T>
using StepResult_t = typename StepResult<T>::type;

template <typename Derived>
constexpr Derived ComputeValue(const Pie<Derived>& x, std::true_type) {
    const auto result = x.derived();
    LOG("ComputeValue(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr auto ComputeValue(const Pie<Derived>& x, std::false_type) {
    LOG("ComputeValue(" << x.derived() << ") ...");
    const auto result = ComputeValue(Step(x.derived()));
    LOG("ComputeValue(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr auto ComputeValue(const Pie<Derived>& x) {
    return ComputeValue(x.derived(), is_value<Derived>{});
}

template <typename Derived>
constexpr Derived Normalize(const Pie<Derived>& x, std::true_type /*is_normal*/) {
    const auto result = x.derived();
    LOG("Normalize(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr auto Normalize(const Pie<Derived>& x, std::false_type /*is_normal*/) {
    LOG("Normalize(" << x.derived() << ") ...");
    const auto result = Normalize(Step(x.derived()));
    LOG("Normalize(" << x.derived() << "): " << result);
    return result;
}

template <typename Derived>
constexpr auto Normalize(const Pie<Derived>& x) {
    return Normalize(x.derived(), is_normal<Derived>{});
}

template <typename F>
struct NormalizedLambda {
    constexpr NormalizedLambda(const F& f) : f_{f} {}

    template <typename Arg>
    constexpr auto operator()(const Arg& arg) const {
        return Normalize(f_(arg));
    }

    F f_;
};

template <typename T, typename Enabled = void>
struct Synth;

template <typename T>
using synth_t = typename Synth<T>::type;

template <typename T, typename = std::void_t<>>
struct can_synth : std::false_type {};

template <typename T>
constexpr inline bool can_synth_v = can_synth<T>::value;

template <typename T>
struct can_synth<T, std::void_t<decltype(synth1(std::declval<T>()))>> : std::true_type {};

// template <typename Derived>
// constexpr auto synth(const Pie<Derived>& x, std::enable_if_t<can_synth_v<Derived>, int> = 0) {
//    LOG("synth(" << x.derived() << ") ...");
//    const auto result = synth1(x.derived());
//    LOG("synth(" << x.derived() << "): " << result);
//    return result;
//}

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

template <typename Derived>
struct is_a_type;

template <typename Derived>
constexpr inline bool is_a_type_v = is_a_type<Derived>::value;

template <typename Derived>
constexpr bool IsAType(const Pie<Derived>&) {
    return is_a_type_v<Derived>;
}

template <typename Type>
struct is_a_type_impl;

template <typename Expr, typename Type>
struct is_a_impl;

template <typename Expr, typename Type>
struct is_a : std::bool_constant<is_a_type_v<Type> && is_a_impl<Expr, Type>::value> {};

template <typename Derived1, typename Derived2>
constexpr inline bool is_a_v = is_a<Derived1, Derived2>::value;

template <typename Derived1, typename Derived2>
constexpr bool IsA(const Pie<Derived1>&, const Pie<Derived2>&) {
    return is_a_v<Derived1, Derived2>;
}

template <typename Expr, bool is_value>
struct compute_value;

template <typename T>
using compute_value_t = typename compute_value<T, is_value_v<T>>::type;

template <typename Expr>
struct compute_value<Expr, true> {
    using type = Expr;
};

template <typename Expr>
struct compute_value<Expr, false> {
    using type = compute_value_t<StepResult_t<Expr>>;
};

template <typename Expr, bool is_normal>
struct normalize;

template <typename T>
using normalize_t = typename normalize<T, is_normal_v<T>>::type;

template <typename Expr>
struct normalize<Expr, true> {
    using type = Expr;
};

template <typename Expr>
struct normalize<Expr, false> {
    using type = normalize_t<StepResult_t<Expr>>;
};

template <typename Derived>
struct is_a_type : std::bool_constant<is_a_type_impl<compute_value_t<Derived>>::value> {};

template <typename Derived1, typename Derived2, typename Derived3>
struct is_the_same_as;

template <typename Derived1, typename Derived2, typename Derived3>
constexpr inline bool is_the_same_as_v = is_the_same_as<Derived1, Derived2, Derived3>::value;

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheSameAs(const Pie<Derived1>&, const Pie<Derived2>&, const Pie<Derived3>&) {
    return is_the_same_as_v<Derived1, Derived2, Derived3>;
}

template <typename Derived1, typename Derived2>
struct is_the_same_type_as;

template <typename Derived1, typename Derived2>
constexpr inline bool is_the_same_type_as_v = is_the_same_type_as<Derived1, Derived2>::value;

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType(const Pie<Derived1>&, const Pie<Derived2>&) {
    return is_the_same_type_as_v<Derived1, Derived2>;
}

template <typename Expr, typename Type>
struct is_a_impl : std::bool_constant<is_the_same_type_as_v<Type, synth_t<Expr>>> {};

template <typename Type, typename Expr1, typename Expr2>
struct is_the_same_as : std::bool_constant<is_a_type_v<Type> && is_a_v<Expr1, Type> && is_a_v<Expr2, Type> &&
                                           equal_v<normalize_t<Expr1>, normalize_t<Expr2>>> {};

template <typename Type1, typename Type2>
struct is_the_same_type_as
    : std::bool_constant<is_a_type_v<Type1> && is_a_type_v<Type2> && equal_v<normalize_t<Type1>, normalize_t<Type2>>> {
};

template <typename Expr, typename Type>
constexpr bool IsANormal(const Pie<Expr>&, const Pie<Type>&) {
    return is_a_v<Expr, Type> && is_normal_v<Expr>;
}

template <typename Type, typename Expr>
constexpr bool IsAValue(const Pie<Type>&, const Pie<Expr>&) {
    return is_a_v<Expr, Type> && is_value_v<Expr>;
}

template <typename Derived1, typename Derived2>
constexpr bool IsNormalFormOfType(const Pie<Derived1>&, const Pie<Derived2>&) {
    LOG("IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << ") ...");
    const auto identical = equal_v<normalize_t<Derived2>, Derived1>;
    LOG("IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << "):" << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsNormalFormOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>&, const Pie<Derived3>&) {
    LOG("IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto identical = equal_v<normalize_t<Derived3>, Derived2>;
    LOG("IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << "): " << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheValueOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>&, const Pie<Derived3>&) {
    LOG("IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto identical = equal_v<compute_value_t<Derived3>, Derived2>;
    LOG("IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << "): " << identical);
    return identical;
}

template <typename Symbol, typename Type, typename Expr>
struct Definition : Pie<Definition<Symbol, Type, Expr>> {
    constexpr Definition(const Type& type, const Expr& expr) : type_{type}, expr_{expr} {}

    Type type_;
    Expr expr_;
};

template <typename Symbol, typename Type, typename Expr>
struct Height<Definition<Symbol, Type, Expr>> : std::integral_constant<int, std::max(height_v<Type>, height_v<Expr>)> {
};

template <typename Symbol1, typename Type1, typename Expr1, typename Symbol2, typename Type2, typename Expr2>
struct Equal<Definition<Symbol1, Type1, Expr1>, Definition<Symbol2, Type2, Expr2>>
    : std::bool_constant<equal_v<Expr1, Expr2>> {};

template <typename Symbol, typename Type, typename Expr, typename T>
struct Equal<Definition<Symbol, Type, Expr>, T> : std::bool_constant<equal_v<Expr, T>> {};

template <typename T, typename Symbol, typename Type, typename Expr>
struct Equal<T, Definition<Symbol, Type, Expr>> : std::bool_constant<equal_v<T, Expr>> {};

template <typename Symbol, typename Type, typename Expr>
struct Printer<Definition<Symbol, Type, Expr>> {
    static void print(std::ostream& s) { s << Symbol::value; }
};

template <typename Symbol, typename Type, typename Expr>
constexpr Definition<Symbol, Type, Expr> define(const Type& type, const Expr& expr) {
    LOG("checking define " << Symbol::value << " ...");
    static_assert(is_a_v<Expr, Type>);
    LOG("checking define " << Symbol::value << " done");
    return Definition<Symbol, Type, Expr>(type, expr);
}

template <typename Symbol, typename Type, typename Expr>
struct StepResult<Definition<Symbol, Type, Expr>> {
    using type = Expr;
};

template <typename Symbol, typename Type, typename Expr>
struct Synth<Definition<Symbol, Type, Expr>> {
    using type = Type;
};

struct U_t : Pie<U_t> {};

template <>
struct Height<U_t> : std::integral_constant<int, 0> {};

template <>
struct Equal<U_t, U_t> : std::true_type {};

template <>
struct Printer<U_t> {
    static void print(std::ostream& s) { s << 'U'; }
};

inline constexpr U_t U;

template <>
struct is_normal<U_t> : std::true_type {};

template <>
struct is_value<U_t> : std::true_type {};

template <>
struct is_a_type_impl<U_t> : std::true_type {};

template <typename Type>
struct is_a_impl<U_t, Type> : std::false_type {};

template <typename Type>
struct is_a_type_impl : std::bool_constant<is_a_v<Type, U_t>> {};
