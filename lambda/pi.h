#pragma once

#include "pie_base.h"
#include "var/typed_var.h"
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

template <typename ArgType, typename Result>
struct Height<Pi_t<ArgType, Result>>
    : std::integral_constant<
          int,
          std::max(height_v<ArgType>, height_v<std::invoke_result_t<Result, TypedVar_t<ArgType, 0>>>) + 1> {};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
struct Equal<Pi_t<ArgType1, Result1>, Pi_t<ArgType2, Result2>>
    : std::bool_constant<
          equal_v<ArgType1, ArgType2> && height_v<Pi_t<ArgType1, Result1>> == height_v<Pi_t<ArgType2, Result2>> &&
          equal_v<std::invoke_result_t<Result1, TypedVar_t<ArgType1, height_v<Pi_t<ArgType1, Result1>>>>,
                  std::invoke_result_t<Result2, TypedVar_t<ArgType2, height_v<Pi_t<ArgType2, Result2>>>>>> {};

template <typename ArgType, typename Result>
struct Printer<Pi_t<ArgType, Result>> {
    static void print(std::ostream& s) {
        using VarType = TypedVar_t<ArgType, height_v<Pi_t<ArgType, Result>>>;
        s << "(Π (" << Print<VarType>{} << ' ' << Print<ArgType>{} << ") "
          << Print<std::invoke_result_t<Result, VarType>>{} << ')';
    }
};

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
    const auto v = var<height_v<Pi_t<Arg, Result>>>(type.arg_);
    return IsAType(type.arg_) && IsAType(type.result_(v));
}

template <typename Arg, typename Result>
struct is_normal<Pi_t<Arg, Result>>
    : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, TypedVar_t<Arg, 0>>>> {};

template <typename Arg, typename Result>
struct is_value<Pi_t<Arg, Result>> : std::true_type {};

template <typename Arg, typename Result>
constexpr U_t synth1(const Pi_t<Arg, Result>& t) {
    assert(IsAType(t));
    return U;
}

template <typename Arg, typename Result>
constexpr auto Normalize(const Pi_t<Arg, Result>& type, std::false_type /*is_normal*/) {
    return Pi(Normalize(type.arg_), NormalizedLambda{type.result_});
}
