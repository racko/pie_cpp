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
    // LOG("StepApp(F=" << value.f_ << ", Arg=" << value.arg_ << ") ...");
    const auto result = ComputeValue(value.f_).f_(value.arg_);
    // LOG("StepApp(F=" << value.f_ << ", Arg=" << value.arg_ << "): " << result);
    return result;
}

template <typename F, typename Arg>
struct is_neutral<App_t<F, Arg>> : std::bool_constant<is_neutral_v<F> && is_normal_v<Arg>> {};

template <typename F, typename Arg>
struct synth_result<App_t<F, Arg>> {
    using type = std::invoke_result_t<typename compute_value_result_t<synth_result_t<F>>::result_type, Arg>;
};

template <typename F, typename Arg>
constexpr synth_result_t<App_t<F, Arg>> synth1(const App_t<F, Arg>& x, int& next_index) {
    LOG("synth(" << x << ") ...");
    const auto pi_type = ComputeValue(synth1(x.f_, next_index));
    assert(IsA1(x.arg_, ComputeValue(pi_type.arg_), next_index));
    const auto& result = pi_type.result_(x.arg_);
    LOG("synth(" << x << "): " << result);

    return result;
}

template <typename F, typename Arg, bool is_f_neutral>
struct normalize_app_result;

template <typename F, typename Arg, bool is_f_neutral>
using normalize_app_result_t = typename normalize_app_result<F, Arg, is_f_neutral>::type;

template <typename F, typename Arg>
struct normalize_app_result<F, Arg, true> {
    using type = App_t<normalize_result_t<F>, normalize_result_t<Arg>>;
};

template <typename F, typename Arg>
constexpr normalize_app_result_t<F, Arg, true> NormalizeApp(const App_t<F, Arg>& app, std::true_type /*is_f_neutral*/) {
    return Normalize(app.f_)(Normalize(app.arg_));
}

template <typename F, typename Arg>
struct normalize_app_result<F, Arg, false> {
    using type = typename normalize_result<step_result_t<App_t<F, Arg>>>::type;
};

template <typename F, typename Arg>
constexpr normalize_app_result_t<F, Arg, false> NormalizeApp(const App_t<F, Arg>& app, std::false_type /*is_f_neutral*/) {
    return Normalize(Step(app.derived()));
}

template <typename F, typename Arg>
struct normalize_result1<App_t<F, Arg>, false> {
    using type = normalize_app_result_t<F, Arg, is_neutral_v<F>>;
};

// !normal => !neutral => !(neutral(f) && normal(arg)) => !neutral(f) || !normal(arg)
template <typename F, typename Arg>
constexpr normalize_result_t<App_t<F, Arg>> Normalize(const App_t<F, Arg>& app, std::false_type /*is_normal*/) {
    return NormalizeApp(app.derived(), is_neutral<F>{});
}
