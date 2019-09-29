#pragma once

#include "lambda/pi.h"
#include "pie_base.h"
#include "var/typed_var.h"
#include "var/var.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename F>
struct Lambda_t : Pie<Lambda_t<F>> {
    using type = F;

    constexpr Lambda_t(const F& f) : f_{f} {}

    // operator()'s declaration can be found in Pie<>
    // The implementation is in lambda/app.h -.-*

    F f_;
};

template <typename F>
struct Height<Lambda_t<F>> : std::integral_constant<int, height_v<std::invoke_result_t<F, Var_t<0>>> + 1> {};

template <typename F1, typename F2>
struct Equal<Lambda_t<F1>, Lambda_t<F2>>
    : std::bool_constant<height_v<Lambda_t<F1>> == height_v<Lambda_t<F2>> &&
                         equal_v<std::invoke_result_t<F1, Var_t<height_v<Lambda_t<F1>>>>,
                                 std::invoke_result_t<F2, Var_t<height_v<Lambda_t<F2>>>>>> {};

template <typename F>
struct Printer<Lambda_t<F>> {
    void print(std::ostream& s) {
        using VarType = Var_t<height_v<Lambda_t<F>>>;
        s << "(λ (" << Print<VarType>{} << ") " << Print<std::invoke_result_t<F, VarType>>{} << ')';
    }
};

template <typename F>
constexpr Lambda_t<F> lambda(const F& f, std::enable_if_t<std::is_invocable_v<F, Var_t<0>>, int> = 0) {
    return Lambda_t<F>(f);
}

template <typename F, typename Arg>
struct BindFront {
    constexpr BindFront(F f, Arg arg) : f_{std::move(f)}, arg_{std::move(arg)} {}

    template <typename... Args, std::enable_if_t<std::is_invocable_v<F, Arg, Args...>, int> = 0>
    constexpr std::invoke_result_t<F, Arg, Args...> operator()(const Args&... args) const {
        return f_(arg_, args...);
    }

    F f_;
    Arg arg_;
};

// std::bind_front isn't constexpr
template <typename F, typename Arg>
constexpr BindFront<F, Arg> bind_front(F f, Arg arg) {
    return BindFront<F, Arg>{std::move(f), std::move(arg)};
}

template <typename F>
constexpr auto lambda(const F& f, std::enable_if_t<!std::is_invocable_v<F, Var_t<0>>, int> = 0) {
    return lambda([f](const auto& arg) { return lambda(bind_front(f, arg)); });
}

template <typename F, typename Arg, typename Result>
constexpr bool IsA1(const Lambda_t<F> f, Pi_t<Arg, Result> type) {
    const auto height = std::max(height_v<Lambda_t<F>>, height_v<Pi_t<Arg, Result>>);
    const auto v = var<height>(type.arg_);
    return IsA(f.f_(v), type.result_(v));
}

template <typename F>
struct is_normal<Lambda_t<F>> : std::bool_constant<is_normal_v<std::invoke_result_t<F, Var_t<0>>>> {};

template <typename F>
struct is_value<Lambda_t<F>> : std::true_type {};

template <typename F>
struct normalize_result1<Lambda_t<F>, false> {
    using type = Lambda_t<NormalizedLambda<F>>;
};

template <typename F>
constexpr normalize_result_t<Lambda_t<F>> Normalize(const Lambda_t<F>& f, std::false_type /*is_normal*/) {
    return lambda(NormalizedLambda{f.f_});
}
