#pragma once

#include "pie_base.h"
#include "var/typed_var.h"
#include <ostream>
#include <type_traits>

template <typename ArgType, typename Result>
struct Sigma_t : Pie<Sigma_t<ArgType, Result>> {
    using arg_type = ArgType;
    using result_type = Result;

    constexpr Sigma_t(const ArgType& arg, const Result& result) : arg_{arg}, result_{result} {}

    ArgType arg_;
    Result result_;
};

template <typename Derived1, typename Derived2>
struct Height<Sigma_t<Derived1, Derived2>>
    : std::integral_constant<
          int,
          std::max(height_v<Derived1>, height_v<std::invoke_result_t<Derived2, TypedVar_t<Derived1, 0>>>) + 1> {};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
constexpr bool equal(const Sigma_t<ArgType1, Result1>& lhs, const Sigma_t<ArgType2, Result2>& rhs) {
    constexpr int height = height_v<Sigma_t<ArgType1, Result1>>;
    if (!(lhs.arg_ == rhs.arg_) || height != height_v<Sigma_t<ArgType2, Result2>>) {
        return false;
    }
    const auto v = var<height>(lhs.arg_);
    return lhs.result_(v) == rhs.result_(v);
}

template <typename ArgType, typename Result>
void print(std::ostream& s, const Sigma_t<ArgType, Result>& type) {
    const auto v = var<height_v<Sigma_t<ArgType, Result>>>(type.arg_);
    s << "(Σ (" << v << ' ' << type.arg_ << ") " << type.result_(v) << ')';
}

template <typename ArgType, typename Result>
constexpr Sigma_t<ArgType, Result> Sigma(const Pie<ArgType>& arg, const Result& result) {
    return Sigma_t<ArgType, Result>{arg.derived(), result};
}

template <typename Arg, typename Result>
constexpr bool IsAType1(const Sigma_t<Arg, Result>& type) {
    const auto v = var<height_v<Sigma_t<Arg, Result>>>(type.arg_);
    return IsAType(type.arg_) && IsAType(type.result_(v));
}

template <typename Arg, typename Result>
struct is_normal<Sigma_t<Arg, Result>>
    : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, TypedVar_t<Arg, 0>>>> {};

template <typename Arg, typename Result>
struct is_value<Sigma_t<Arg, Result>> : std::true_type {};

template <typename Arg, typename Result>
struct synth_result<Sigma_t<Arg, Result>> {
    using type = U_t;
};

template <typename Arg, typename Result>
constexpr synth_result_t<Sigma_t<Arg, Result>> synth1(const Sigma_t<Arg, Result>& t) {
    assert(IsAType(t));
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
