#pragma once

#include "pie_base.h"
#include "var/typed_var.h"
#include "var/var.h"
#include <ostream>
#include <type_traits>

template <typename ArgType, typename Result>
struct Sigma_t : Pie<Sigma_t<ArgType, Result>> {
    using arg_type = ArgType;
    using result_type = Result;

    constexpr Sigma_t(const ArgType& arg, const Result& result) : arg_{arg}, result_{result} {}

    ArgType arg_;
    Result result_; // result_ is now a function object
};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
constexpr bool equal(const Sigma_t<ArgType1, Result1>& lhs, const Sigma_t<ArgType2, Result2>& rhs, int& next_index) {
    const Var_t var{next_index++};
    return equal(lhs.arg_, rhs.arg_, next_index) && equal(lhs.result_(var), rhs.result_(var), next_index);
}

template <typename ArgType, typename Result>
void print(std::ostream& s, const Sigma_t<ArgType, Result>& type, int& next_index) {
    const Var_t var{next_index++};
    s << "(Σ (" << var << ' ' << InContext(type.arg_, next_index) << ") " << InContext(type.result_(var), next_index)
      << ')';
}

template <typename ArgType, typename Result>
constexpr Sigma_t<ArgType, Result> Sigma(const Pie<ArgType>& arg, const Result& result) {
    return Sigma_t<ArgType, Result>{arg.derived(), result};
}

template <typename Arg, typename Result>
constexpr bool IsAType1(const Sigma_t<Arg, Result>& type, int& next_index) {
    const auto v = var(type.arg_, next_index++);
    return IsAType1(type.arg_, next_index) && IsAType1(type.result_(v), next_index);
}

template <typename Arg, typename Result>
struct is_normal<Sigma_t<Arg, Result>>
    : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, Var_t>>> {};

template <typename Arg, typename Result>
struct is_value<Sigma_t<Arg, Result>> : std::true_type {};

template <typename Arg, typename Result>
struct synth_result<Sigma_t<Arg, Result>> {
    using type = U_t;
};

template <typename Arg, typename Result>
constexpr synth_result_t<Sigma_t<Arg, Result>> synth1(Sigma_t<Arg, Result>, int&) {
    return U;
}

template <typename Arg, typename Result>
struct normalize_result1<Sigma_t<Arg, Result>, false> {
    using type = Sigma_t<normalize_result_t<Arg>, NormalizedLambda<Result>>;
};

template <typename Arg, typename Result>
constexpr normalize_result_t<Sigma_t<Arg, Result>> Normalize(const Sigma_t<Arg, Result>& type,
                                                             std::false_type /*is_normal*/) {
    return Sigma(Normalize(type.arg_), NormalizedLambda{type.result_});
}
