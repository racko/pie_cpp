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

// TODO:
// - make test1.cpp green
//   - update pi
//     - replace synth1
//     - replace Normalize
//   - update sigma
// - make other tests green

template <typename Arg, typename Result>
struct is_a_type_impl<Pi_t<Arg, Result>>
    : std::bool_constant<is_a_type_v<Arg> &&
                         is_a_type_v<std::invoke_result_t<Result, TypedVar_t<Arg, height_v<Pi_t<Arg, Result>>>>>> {};

template <typename Arg, typename Result>
struct is_normal<Pi_t<Arg, Result>>
    : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, TypedVar_t<Arg, 0>>>> {};

template <typename Arg, typename Result>
struct is_value<Pi_t<Arg, Result>> : std::true_type {};

template <typename Arg, typename Result>
struct Synth<Pi_t<Arg, Result>> {
    static_assert(is_a_type_v<Pi_t<Arg, Result>>);
    using type = U_t;
};

constexpr auto Normalize(const Pi_t<Arg, Result>& type, std::false_type /*is_normal*/) {
    return Pi(Normalize(type.arg_), NormalizedLambda{type.result_});
}
