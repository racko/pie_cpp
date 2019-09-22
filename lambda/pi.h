#pragma once

#include "pie_base.h"
#include "var/typed_var.h"
#include "var/var.h"
#include <ostream>
#include <type_traits>

template <typename ArgType, typename Result>
struct Pi_t : Pie<Pi_t<ArgType, Result>> {
    using arg_type = ArgType;
    using result_type = Result;

    constexpr Pi_t(const ArgType& arg, const Result& result)
        : height_{std::max(arg.height_, result(var(0)).height_) + 1}, arg_{arg}, result_{result} {}

    int height_;
    ArgType arg_;
    Result result_;
};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
constexpr bool equal(const Pi_t<ArgType1, Result1>& lhs, const Pi_t<ArgType2, Result2>& rhs) {
    const Var_t var{std::max(lhs.height_, rhs.height_)};
    return equal(lhs.arg_, rhs.arg_) && equal(lhs.result_(var), rhs.result_(var));
}

template <typename ArgType, typename Result>
void print(std::ostream& s, const Pi_t<ArgType, Result>& type) {
    const auto v = var(type.arg_, type.height_);
    s << "(Π (" << v << ' ' << type.arg_ << ") " << type.result_(v) << ')';
}

template <typename ArgType, typename Result>
constexpr Pi_t<ArgType, Result> Pi(const Pie<ArgType>& arg, const Result& result) {
    return Pi_t<ArgType, Result>{arg.derived(), result};
}

template <typename Result>
constexpr Result Arrow(const Pie<Result>& result) {
    return result.derived();
}

template <typename Arg, typename... Args>
constexpr auto Arrow(const Pie<Arg>& arg, const Args&... args) {
    return Pi(arg.derived(), [args...](const auto&) { return Arrow(args...); });
}

template <typename Arg, typename Result>
constexpr bool IsAType1(const Pi_t<Arg, Result>& type) {
    const auto v = var(type.arg_, type.height_);
    return IsAType(type.arg_) && IsAType(type.result_(v));
}

template <typename Arg, typename Result>
struct is_normal<Pi_t<Arg, Result>>
    : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, Var_t>>> {};

template <typename Arg, typename Result>
struct is_value<Pi_t<Arg, Result>> : std::true_type {};

template <typename Arg, typename Result>
struct synth_result<Pi_t<Arg, Result>> {
    using type = U_t;
};

template <typename Arg, typename Result>
constexpr synth_result_t<Pi_t<Arg, Result>> synth1(const Pi_t<Arg, Result>& t) {
    assert(IsAType(t));
    return U;
}

template <typename Arg, typename Result>
struct normalize_result1<Pi_t<Arg, Result>, false> {
    using type = Pi_t<normalize_result_t<Arg>, NormalizedLambda<Result>>;
};

template <typename Arg, typename Result>
constexpr normalize_result_t<Pi_t<Arg, Result>> Normalize(const Pi_t<Arg, Result>& type,
                                                          std::false_type /*is_normal*/) {
    return Pi(Normalize(type.arg_), NormalizedLambda{type.result_});
}
