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

    constexpr Pi_t(const ArgType& arg, const Result& result) : arg_{arg}, result_{result} {}

    ArgType arg_;
    Result result_;
};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
constexpr bool equal(const Pi_t<ArgType1, Result1>& lhs, const Pi_t<ArgType2, Result2>& rhs, int& next_index) {
    const Var_t var{next_index++};
    return equal(lhs.arg_, rhs.arg_, next_index) && equal(lhs.result_(var), rhs.result_(var), next_index);
}

template <typename ArgType, typename Result>
void print(std::ostream& s, const Pi_t<ArgType, Result>& type, int& next_index) {
    const auto v = var(type.arg_, next_index++);
    s << "(Π (" << v << ' ' << InContext(type.arg_, next_index) << ") " << InContext(type.result_(v), next_index)
      << ')';
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
constexpr bool IsAType1(const Pi_t<Arg, Result>& type, int& next_index) {
    const auto v = var(type.arg_, next_index++);
    return IsAType1(type.arg_, next_index) && IsAType1(type.result_(v), next_index);
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
constexpr synth_result_t<Pi_t<Arg, Result>> synth1(Pi_t<Arg, Result>, int&) {
    return U;
}

template <typename Arg, typename Result>
struct normalize_result1<Pi_t<Arg, Result>, false> {
    using type = Pi_t<normalize_result_t<Arg>, NormalizedLambda<Result>>;
};

template <typename Arg, typename Result>
constexpr normalize_result_t<Pi_t<Arg, Result>> Normalize(const Pi_t<Arg, Result>& type, std::false_type /*is_normal*/) {
    return Pi(Normalize(type.arg_), NormalizedLambda{type.result_});
}
