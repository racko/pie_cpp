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
    // The implementation is above -.-*

    F f_;
};

template <typename F1, typename F2>
constexpr bool equal(const Lambda_t<F1>& lhs, const Lambda_t<F2>& rhs, int& next_index) {
    const Var_t var{next_index++};
    return equal(lhs.f_(var), rhs.f_(var), next_index);
}

template <typename F>
void print(std::ostream& s, const Lambda_t<F>& f, int& next_index) {
    const Var_t var{next_index++};
    s << "(λ (" << var << ") " << InContext(f.f_(var), next_index) << ')';
}

template <typename F>
constexpr Lambda_t<F> lambda(const F& f, std::enable_if_t<std::is_invocable_v<F, Var_t>, int> = 0) {
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

template <typename F, typename Arg>
constexpr BindFront<F, Arg> bind_front(F f, Arg arg) {
    return BindFront<F, Arg>{std::move(f), std::move(arg)};
}

template <typename F>
constexpr auto lambda(const F& f, std::enable_if_t<!std::is_invocable_v<F, Var_t>, int> = 0) {
    return lambda([f](const auto& arg) { return lambda(bind_front(f, arg)); });
}

template <typename Arg, typename Result, typename F>
constexpr bool IsA1(const Lambda_t<F> f, Pi_t<Arg, Result> type, int& next_index) {
    const auto v = var(type.arg_, next_index++);
    return IsA1(f.f_(v), ComputeValue(type.result_(v)), next_index);
}

template <typename F>
struct is_normal<Lambda_t<F>> : std::bool_constant<is_normal_v<std::invoke_result_t<F, Var_t>>> {};

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
