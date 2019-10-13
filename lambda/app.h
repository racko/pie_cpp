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

template <typename F, typename Arg>
struct Height<App_t<F, Arg>> : std::integral_constant<int, std::max(height_v<F>, height_v<Arg>)> {};

template <typename F>
constexpr F apply(const Pie<F>& f) {
    return f.derived();
}

template <typename F, typename Arg, typename... Args>
constexpr auto apply(const Pie<F>& f, const Pie<Arg>& arg, const Pie<Args>&... args) {
    return ::apply(App_t<F, Arg>{f.derived(), arg.derived()}, args.derived()...);
}

template <typename Derived>
template <typename... Args>
constexpr auto Pie<Derived>::operator()(const Pie<Args>&... args) const {
    return ::apply(derived(), args.derived()...);
}

template <typename F1, typename Arg1, typename F2, typename Arg2>
struct Equal<App_t<F1, Arg1>, App_t<F2, Arg2>> : std::bool_constant<equal_v<F1, F2> && equal_v<Arg1, Arg2>> {};

template <typename F, typename Arg>
struct Printer<App_t<F, Arg>> {
    static void print(std::ostream& s) { s << '(' << Print<F>{} << ' ' << Print<Arg>{} << ')'; }
};

template <typename F, typename Arg>
constexpr auto Step1(const App_t<F, Arg>& value) {
    return ComputeValue(value.f_).f_(value.arg_);
}

template <typename F, typename Arg>
struct is_neutral<App_t<F, Arg>> : std::bool_constant<is_neutral_v<F> && is_normal_v<Arg>> {};

template <typename F, typename Arg, typename = std::enable_if_t<can_synth_v<F>>>
constexpr auto synth1(const App_t<F, Arg>& x) {
    const auto pi_type = ComputeValue(synth(x.f_));
    assert(IsA(x.arg_, pi_type.arg_));
    return pi_type.result_(x.arg_);
}
