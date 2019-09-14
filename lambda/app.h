#pragma once

#include "pie_base.h"
#include <ostream>
#include <type_traits>

template <typename F, typename Arg>
struct App_t : Pie<App_t<F, Arg>> {
    constexpr App_t(const F& f, const Arg& arg) : f_{f}, arg_{arg} {}

    F f_;
    Arg arg_;
};

template <typename F>
constexpr apply_result_t<F> apply(const Pie<F>& f) {
    return f.derived();
}

template <typename F, typename Arg, typename... Args>
constexpr apply_result_t<F, Arg, Args...> apply(const Pie<F>& f, const Pie<Arg>& arg, const Pie<Args>&... args) {
    return ::apply(App_t<F, Arg>{f.derived(), arg.derived()}, args.derived()...);
}

template <typename Derived>
template <typename... Args>
constexpr apply_result_t<Derived, Args...> Pie<Derived>::operator()(const Pie<Args>&... args) const {
    return ::apply(derived(), args.derived()...);
}

template <typename F1, typename Arg1, typename F2, typename Arg2>
constexpr bool equal(const App_t<F1, Arg1>& lhs, const App_t<F2, Arg2>& rhs, int& next_index) {
    return equal(lhs.f_, rhs.f_, next_index) && equal(lhs.arg_, rhs.arg_, next_index);
}

template <typename F, typename Arg>
void print(std::ostream& s, const App_t<F, Arg>& app, int& next_index) {
    s << '(' << InContext(app.f_, next_index) << ' ' << InContext(app.arg_, next_index) << ')';
}

template <typename F, typename Arg>
struct step_result<App_t<F, Arg>> {
    using type = std::invoke_result_t<typename compute_value_result_t<F>::type, Arg>;
};

template <typename F, typename Arg>
constexpr step_result_t<App_t<F, Arg>> Step(const App_t<F, Arg>& value) {
    return ComputeValue(value.f_).f_(value.arg_);
}

template <typename F, typename Arg>
struct is_neutral<App_t<F, Arg>> : std::bool_constant<is_neutral_v<F> && is_normal_v<Arg>> {};

template <typename F, typename Arg>
struct synth_result<App_t<F, Arg>> {
    using type = std::invoke_result_t<typename synth_result_t<F>::result_type, Arg>;
};

template <typename F, typename Arg>
constexpr synth_result_t<App_t<F, Arg>> synth1(const App_t<F, Arg>& x, int& next_index) {
    const auto pi_type = synth1(x.f_, next_index);
    assert(IsA1(x.arg_, pi_type.arg_, next_index));
    return pi_type.result_(x.arg_);
}

