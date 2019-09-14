#pragma once

#include <cassert>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>

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

template <typename Derived1, typename Derived2>
constexpr bool
equal([[maybe_unused]] const Pie<Derived1>& lhs, [[maybe_unused]] const Pie<Derived2>& rhs, int& /*next_index*/) {
    // std::cerr << "operator==(" << lhs.derived() << ", " << rhs.derived() << "): false (default)\n";
    return false;
}

template <typename Derived1, typename Derived2>
constexpr bool operator==(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    int next_index{0};
    return equal(lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived>
struct InContext {
    InContext(const Derived& expr, int& next_index) : expr_{expr}, next_index_{next_index} {}

    const Derived& expr_;
    int& next_index_;
};

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
    // std::cerr << "ComputeValue(" << x.derived() << ", is_value=true) ...\n";
    return x.derived();
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x, std::false_type) {
    // std::cerr << "ComputeValue(" << x.derived() << ", is_value=false) ...\n";
    return ComputeValue(Step(x.derived()));
}

template <typename Derived>
constexpr compute_value_result_t<Derived> ComputeValue(const Pie<Derived>& x) {
    // std::cerr << "ComputeValue(" << x.derived() << ") ...\n";
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
    // std::cerr << "Normalize(" << x.derived() << ", is_normal=true) ...\n";
    return x.derived();
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x, std::false_type /*is_normal*/) {
    // std::cerr << "Normalize(" << x.derived() << ", is_normal=false) ...\n";
    return Normalize(Step(x.derived()));
}

template <typename Derived>
constexpr normalize_result_t<Derived> Normalize(const Pie<Derived>& x) {
    // std::cerr << "Normalize(" << x.derived() << ") ...\n";
    return Normalize(x.derived(), is_normal<Derived>{});
}

template <typename T>
struct synth_result;

template <typename T>
using synth_result_t = typename synth_result<T>::type;

template <typename Derived>
constexpr synth_result_t<Derived> synth(const Pie<Derived>& x) {
    int next_index{0};
    return synth1(x.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsA1(const Pie<Derived1>& value, const Pie<Derived2>& type, int& next_index) {
    // std::cerr << "IsA(" << value.derived() << ", " << type.derived() << ") [default] ... \n";
    return AreTheSameType1(type.derived(), synth1(value.derived(), next_index), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsA(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    int next_index{0};
    return IsA1(value.derived(), type.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsANormal(const Pie<Derived1>& value, const Pie<Derived2>& type) {
    return IsA(value.derived(), type.derived()) && is_normal_v<Derived1>;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool
IsTheSameAs1(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs, int& next_index) {
    // std::cerr << "IsTheSameAs(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto is_type_valid = IsAType1(type.derived(), next_index);
    // std::cerr << "type is valid: " << is_type_valid << '\n';
    if (!is_type_valid) {
        return false;
    }
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    // std::cerr << "lhs normalform: " << lhs_norm << '\n';
    // std::cerr << "rhs normalform: " << rhs_norm << '\n';
    const auto normalforms_identical = equal(lhs_norm, rhs_norm, next_index);
    // std::cerr << "normalforms are identical: " << normalforms_identical << '\n';
    return normalforms_identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheSameAs(const Pie<Derived1>& type, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    int next_index{0};
    return IsTheSameAs1(type.derived(), lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsAValue(const Pie<Derived1>& type, const Pie<Derived2>& expr) {
    return IsA(expr.derived(), type.derived()) && is_value_v<Derived2>;
}

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType1(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs, int& next_index) {
    // std::cerr << "AreTheSameType(" << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto lhs_norm = Normalize(lhs.derived());
    const auto rhs_norm = Normalize(rhs.derived());
    // std::cerr << "lhs normalform: " << lhs_norm << '\n';
    // std::cerr << "rhs normalform: " << rhs_norm << '\n';
    const auto normalforms_identical = equal(lhs_norm, rhs_norm, next_index);
    // std::cerr << "normalforms are identical: " << normalforms_identical << '\n';
    return normalforms_identical;
}

template <typename Derived1, typename Derived2>
constexpr bool AreTheSameType(const Pie<Derived1>& lhs, const Pie<Derived2>& rhs) {
    int next_index{0};
    return AreTheSameType1(lhs.derived(), rhs.derived(), next_index);
}

template <typename Derived1, typename Derived2>
constexpr bool IsNormalFormOfType(const Pie<Derived1>& nf, const Pie<Derived2>& type) {
    // std::cerr << "IsNormalFormOfType(" << nf.derived() << ", " << type.derived() << ") ...\n";
    const auto actual = Normalize(type.derived());
    // std::cerr << "rhs normal form: " << actual << '\n';
    const auto identical = actual == nf.derived();
    // std::cerr << "rhs normal form == lhs: " << identical << '\n';
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsNormalFormOf(const Pie<Derived1>& /*type*/, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    // std::cerr << "IsNormalFormOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ...\n";
    const auto actual = Normalize(rhs.derived());
    // std::cerr << "rhs normal form: " << actual << '\n';
    const auto identical = actual == lhs.derived();
    // std::cerr << "rhs normal form == lhs: " << identical << '\n';
    return identical;
}

template <typename Derived1, typename Derived2, typename Derived3>
constexpr bool IsTheValueOf(const Pie<Derived1>& /*type*/, const Pie<Derived2>& lhs, const Pie<Derived3>& rhs) {
    // std::cerr << "IsTheValueOf(" << type.derived() << ", " << lhs.derived() << ", " << rhs.derived() << ") ... \n";
    const auto rhs_value = ComputeValue(rhs.derived());
    // std::cerr << "rhs value: " << rhs_value << '\n';
    const auto identical = rhs_value == lhs.derived();
    // std::cerr << "rhs value == lhs: " << identical << '\n';
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
    // std::cerr << "checking define " << name << " ...\n";
    assert(IsA(expr.derived(), type.derived()));
    // std::cerr << "checking define " << name << " done\n";
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

constexpr bool IsAType1(U_t, int&) { return true; }

template <typename Derived>
constexpr bool IsA(U_t, [[maybe_unused]] const Pie<Derived>& type) {
    // std::cerr << "IsA(U, " << type.derived() << "): false (U does not have a type) \n";
    return false;
}

template <typename Derived>
constexpr bool IsAType1(const Pie<Derived>& type, int& next_index) {
    return IsA1(type.derived(), U, next_index);
}

template <typename Derived>
constexpr bool IsAType(const Pie<Derived>& type) {
    int next_index{0};
    return IsAType1(type.derived(), next_index);
}
