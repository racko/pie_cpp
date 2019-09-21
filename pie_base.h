#pragma once

#include <cassert>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>

//#define DEBUG

#ifdef DEBUG
#define LOG(msg) do { std::cerr << msg << '\n'; } while(0)
#define FOO inline
#else
#define LOG(msg) do {} while(0)
#define FOO constexpr
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

template <typename Derived>
struct InContext {
    InContext(const Derived& expr, int& next_index) : expr_{expr}, next_index_{next_index} {}

    const Derived& expr_;
    int& next_index_;
};

template <typename Derived1, typename Derived2>
constexpr bool equal([[maybe_unused]] const Pie<Derived1>& lhs,
                     [[maybe_unused]] const Pie<Derived2>& rhs,
                     [[maybe_unused]] int& next_index) {
    LOG("operator==(" << lhs.derived() << ", " << rhs.derived() << "): false (default)");
    return false;
}

template <typename Derived1, typename Derived2>
constexpr bool operator==(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    int next_index{0};
    return equal(lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const InContext<Derived>& expr) {
    print(s, expr.expr_, expr.next_index_);
    return s;
}

template <typename Derived>
std::ostream& operator<<(std::ostream& s, const Pie<Derived>& expr) {
    int next_index{0};
    print(s, expr.derived(), next_index);
    return s;
}

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
step_result_t<Derived> Step(const Pie<Derived>&);

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
    LOG("ComputeValue(" << x.derived() << ", is_value=true) ...");
    return x.derived();
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::false_type) {
    LOG("ComputeValue(" << x.derived() << ", is_value=false) ...");
    return ComputeValue(Step(x.derived()));
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x) {
    LOG("ComputeValue(" << x.derived() << ") ...");
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
    LOG("Normalize(" << x.derived() << ", is_normal=true) ...");
    return x.derived();
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::false_type /*is_normal*/) {
    LOG("Normalize(" << x.derived() << ", is_normal=false) ...");
    return Normalize(Step(x.derived()));
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x) {
    LOG("Normalize(" << x.derived() << ") ...");
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

template <typename T>
struct synth_result;

template <typename T>
using synth_result_t = typename synth_result<T>::type;

template <typename Derived>
constexpr synth_result_t<Derived> synth(const Pie<Derived>& x) {
    int next_index{0};
    return synth1(x.derived(), next_index);
}

template <typename T, typename = std::void_t<>>
struct can_synth : std::false_type {};

template <typename T>
constexpr inline bool can_synth_v = can_synth<T>::value;

template <typename T>
struct can_synth<T, std::void_t<decltype(synth(std::declval<T>()))>> : std::true_type {};

template <typename Derived1, typename Derived2>
constexpr bool IsA1(const Pie<Derived1>& value,
                    const Pie<Derived2>& type,
                    int& next_index,
                    std::enable_if_t<can_synth_v<Derived1>, int> = 0) {
    LOG("IsA(" << value.derived() << ", " << type.derived() << ") [default] ...");
    return AreTheSameType1(type.derived(), synth1(value.derived(), next_index), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsA1([[maybe_unused]] const Pie<Derived1>& value,
                    [[maybe_unused]] const Pie<Derived2>& type,
                    [[maybe_unused]] int& next_index,
                    std::enable_if_t<!can_synth_v<Derived1>, int> = 0) {
    LOG("IsA(" << value.derived() << ", " << type.derived() << "): false [can not synth]");
    return false;
}

template <typename Derived1, typename Derived2>
constexpr bool IsA(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    int next_index{0};
    LOG("IsA(" << value.derived() << ", " << type.derived() << ") ...");
    return IsA1(value.derived(), ComputeValue(type.derived()), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsANormal(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    return IsA(value.derived(), ComputeValue(type.derived())) && is_normal_v<Derived1>;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsTheSameAs1(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs, int& next_index) {
    LOG("IsTheSameAs(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto is_type_valid = IsAType1(type.derived(), next_index);
    LOG("type is valid: " << is_type_valid);
    if (!is_type_valid) {
        return false;
    }
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    LOG("lhs normalform: " << lhs_norm);
    LOG("rhs normalform: " << rhs_norm);
    const auto normalforms_identical = equal(lhs_norm, rhs_norm, next_index);
    LOG("normalforms are identical: " << normalforms_identical);
    return normalforms_identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheSameAs(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    int next_index{0};
    return IsTheSameAs1(type.derived(), lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsAValue(const Pie<Derived1>& type, const Pie<Derived2>& expr) {
    return IsA(expr.derived(), ComputeValue(type.derived())) && is_value_v<Derived2>;
}

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType1(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs, int& next_index) {
    LOG("AreTheSameType(" << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    LOG("lhs normalform: " << lhs_norm);
    LOG("rhs normalform: " << rhs_norm);
    const auto normalforms_identical = equal(lhs_norm, rhs_norm, next_index);
    LOG("normalforms are identical: " << normalforms_identical);
    return normalforms_identical;
}

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    int next_index{0};
    return AreTheSameType1(lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsNormalFormOfType(const Pie<Derived1>& nf, const Pie<Derived2>& type) {
    LOG("IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << ") ...");
    const auto actual = Normalize(type.derived());
    LOG("rhs normal form: " << actual);
    const auto identical = actual == nf.derived();
    LOG("rhs normal form == lhs: " << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsNormalFormOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    LOG("IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto actual = Normalize(rhs.derived());
    LOG("rhs normal form: " << actual);
    const auto identical = actual == lhs.derived();
    LOG("rhs normal form == lhs: " << identical);
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsTheValueOf([[maybe_unused]] const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    LOG("IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...");
    const auto rhs_value = ComputeValue(rhs.derived());
    LOG("rhs value: " << rhs_value);
    const auto identical = rhs_value == lhs.derived();
    LOG("rhs value == lhs: " << identical);
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

template <typename Type1, typename Expr1, typename Type2, typename Expr2>
constexpr bool equal(const Definition<Type1, Expr1>& lhs, const Definition<Type2, Expr2>& rhs, int&) {
    return lhs.name_ == rhs.name_;
}

template <typename Type, typename Expr, typename Derived>
constexpr bool equal(const Definition<Type, Expr>& lhs, const Pie<Derived>& rhs, int& next_index) {
    return equal(lhs.expr_, rhs.derived(), next_index);
}

template <typename Derived, typename Type, typename Expr>
constexpr bool equal(const Pie<Derived>& lhs, const Definition<Type, Expr>& rhs, int& next_index) {
    return equal(lhs.derived(), rhs.expr_, next_index);
}

template <typename Type, typename Expr>
void print(std::ostream& s, const Definition<Type, Expr>& def, int&) {
    s << def.name_;
}

template <typename Type, typename Expr>
constexpr Definition<Type, Expr> define(const char* name, const Type& type, const Expr& expr) {
    LOG("checking define " << name << " ...");
    assert(IsA(expr.derived(), ComputeValue(type.derived())));
    LOG("checking define " << name << " done");
    return Definition(name, type, expr);
}

template <typename Type, typename Expr>
struct step_result<Definition<Type, Expr>> {
    using type = Expr;
};

template <typename Type, typename Expr>
constexpr step_result_t<Definition<Type, Expr>> Step(const Definition<Type, Expr>& x) {
    return x.expr_;
}

template <typename Type, typename Expr>
struct synth_result<Definition<Type, Expr>> {
    using type = Type;
};

template <typename Type, typename Expr>
constexpr synth_result_t<Definition<Type, Expr>> synth1(const Definition<Type, Expr>& x, int&) {
    LOG("synth(" << x << "): " << x.type_);
    return x.type_;
}

struct U_t : Pie<U_t> {};

constexpr bool equal(U_t, U_t, int&) { return true; }

inline void print(std::ostream& s, U_t, int&) { s << 'U'; }

inline constexpr U_t U;

template <>
struct is_normal<U_t> : std::true_type {};

template <>
struct is_value<U_t> : std::true_type {};

FOO bool IsAType1(U_t, int&) {
    LOG("IsAType(U): true");
    return true;
}

template <typename Derived>
constexpr bool IsA(U_t, [[maybe_unused]] const Pie<Derived>& type) {
    LOG("IsA(U, " << type.derived() << "): false (U does not have a type)");
    return false;
}

template <typename Derived>
constexpr bool IsAType1(const Pie<Derived>& type, int& next_index) {
    LOG("IsAType(" << type.derived() << "): ...");
    return IsA1(type.derived(), U, next_index);
}

template <typename Derived>
constexpr bool IsAType(const Pie<Derived>& type) {
    int next_index{0};
    return IsAType1(type.derived(), next_index);
}
