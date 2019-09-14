#pragma once

#include "pie_base.h"
#include "var.h"
#include <functional>
#include <ostream>

template <typename ArgType, typename Result>
struct Pi_t : Pie<Pi_t<ArgType, Result>> {
    using arg_types = ArgType;
    using result_type = Result;

    Pi_t(const ArgType& arg, const Result& result) : arg_{arg}, result_{result} {}

    ArgType arg_;
    Result result_; // result_ is now a function object
};

template <typename ArgType1, typename Result1, typename ArgType2, typename Result2>
bool operator==(const Pi_t<ArgType1, Result1>& lhs, const Pi_t<ArgType2, Result2>& rhs) {
    const Var_t var;
    return lhs.arg_ == rhs.arg_ && lhs.result_(var) == rhs.result_(var);
}

template <typename ArgType, typename Result>
std::wostream& operator<<(std::wostream& s, const Pi_t<ArgType, Result>& type) {
    const Var_t var;
    return s << L"(Π (" << var << ' ' << type.arg_ << ") " << type.result_(var) << ')';
}

template <typename ArgType, typename Result>
Pi_t<ArgType, Result> Pi(const Pie<ArgType>& arg, const Result& result) {
    return Pi_t<ArgType, Result>{arg.derived(), result};
}

//template <typename Arg, typename Result>
//auto Arrow(const Pie<Arg>& arg, const Pie<Result>& result) {
//    return Pi(arg.derived(), [result=result.derived()](const auto&) { return result; });
//}
//
//template <typename Arg, typename... Args>
//auto Arrow(const Pie<Arg>& arg, const Pie<Args>&... args) {
//    return Pi(arg.derived(), [args...](const auto&) { return Arrow(args...); });
//}

template <typename Result>
Result Arrow(const Pie<Result>& result) {
    return result.derived();
}

template <typename Arg, typename... Args>
auto Arrow(const Pie<Arg>& arg, const Args&... args) {
    return Pi(arg.derived(), [args...](const auto&) { return Arrow(args...); });
}

template <typename F, typename Arg>
struct App_t : Pie<App_t<F, Arg>> {
    App_t(const F& f, const Arg& arg) : f_{f}, arg_{arg} {}

    F f_;
    Arg arg_;
};

template <typename F>
apply_result_t<F> apply(const Pie<F>& f) {
    return f.derived();
}

template <typename F, typename Arg, typename... Args>
apply_result_t<F, Arg, Args...> apply(const Pie<F>& f, const Pie<Arg>& arg, const Pie<Args>&... args) {
    return ::apply(App_t<F, Arg>{f.derived(), arg.derived()}, args.derived()...);
}

template <typename Derived>
template <typename... Args>
apply_result_t<Derived, Args...> Pie<Derived>::operator()(const Pie<Args>&... args) const {
    return ::apply(derived(), args.derived()...);
}

template <typename F1, typename Arg1, typename F2, typename Arg2>
bool operator==(const App_t<F1, Arg1>& lhs, const App_t<F2, Arg2>& rhs) {
    return lhs.f_ == rhs.f_ && lhs.arg_ == rhs.arg_;
}

template <typename F, typename Arg>
std::wostream& operator<<(std::wostream& s, const App_t<F, Arg>& app) {
    return s << '(' << app.f_ << ' ' << app.arg_ << ')';
}

template <typename F>
struct Lambda_t : Pie<Lambda_t<F>> {
    using type = F;

    Lambda_t(const F& f) : f_{f} {}

    // operator()'s declaration can be found in Pie<>
    // The implementation is above -.-*

    F f_;
};

template <typename F1, typename F2>
bool operator==(const Lambda_t<F1>& lhs, const Lambda_t<F2>& rhs) {
    const Var_t var;
    return lhs.f_(var) == rhs.f_(var);
}

template <typename F>
std::wostream& operator<<(std::wostream& s, const Lambda_t<F>& f) {
    const Var_t var;
    return s << L"(λ (" << var << ") " << f.f_(var) << ')';
}

template <typename F>
Lambda_t<F> lambda(const F& f, std::enable_if_t<std::is_invocable_v<F, Var_t>, int> = 0) {
    return Lambda_t<F>(f);
}

template <typename F>
auto lambda(const F& f, std::enable_if_t<!std::is_invocable_v<F, Var_t>, int> = 0) {
    return lambda([f](const auto& arg) { return lambda(std::bind_front(f, arg)); });
}

template <typename Arg, typename Result, typename F>
bool IsA(const Lambda_t<F> f, Pi_t<Arg, Result> type) {
    const auto v = var(type.arg_);
    return IsA(f.f_(v), type.result_(v));
}

template <typename Arg, typename Result>
bool IsAType(const Pi_t<Arg, Result>& type) {
    const auto v = var(type.arg_);
    return IsAType(type.arg_) && IsAType(type.result_(v));
}

template <typename Arg, typename Result>
struct is_normal<Pi_t<Arg, Result>> : std::bool_constant<is_normal_v<Arg> && is_normal_v<std::invoke_result_t<Result, Var_t>>> {};

template <typename Arg, typename Result>
struct is_value<Pi_t<Arg, Result>> : std::true_type {};

template <typename F>
struct is_normal<Lambda_t<F>> : std::bool_constant<is_normal_v<std::invoke_result_t<F, Var_t>>> {};

template <typename F>
struct is_value<Lambda_t<F>> : std::true_type {};

template <typename F, typename Arg>
struct step_result<App_t<F, Arg>> {
    using type = std::invoke_result_t<typename compute_value_result_t<F>::type, Arg>;
};

template <typename F, typename Arg>
step_result_t<App_t<F, Arg>> Step(const App_t<F, Arg>& value) {
    return ComputeValue(value.f_).f_(value.arg_);
}

template <typename F, typename Arg>
struct is_neutral<App_t<F, Arg>> : std::bool_constant<is_neutral_v<F> && is_normal_v<Arg>> {};

template <typename F>
struct NormalizedLambda {
    NormalizedLambda(const F& f) : f_{f} {}

    template <typename Arg>
    normalize_result_t<std::invoke_result_t<F, Arg>> operator()(const Arg& arg) const {
        return Normalize(f_(arg));
    }

    F f_;
};

template <typename F>
struct normalize_result1<Lambda_t<F>, false> {
    using type = Lambda_t<NormalizedLambda<F>>;
};

template <typename F>
normalize_result_t<Lambda_t<F>> Normalize(const Lambda_t<F>& f, std::false_type /*is_normal*/) {
    return lambda(NormalizedLambda{f.f_});
}

template <typename Arg, typename Result>
struct synth_result<Pi_t<Arg, Result>> {
    using type = U_t;
};

template <typename Arg, typename Result>
synth_result_t<Pi_t<Arg, Result>> synth(Pi_t<Arg, Result>) {
    return U;
}

template <typename F, typename Arg>
struct synth_result<App_t<F, Arg>> {
    using type = std::invoke_result_t<typename synth_result_t<F>::result_type, Arg>;
};

template <typename F, typename Arg>
synth_result_t<App_t<F, Arg>> synth(const App_t<F, Arg>& x) {
    const auto pi_type = synth(x.f_);
    assert(IsA(x.arg_, pi_type.arg_));
    return pi_type.result_(x.arg_);
}

/// UNUSED

//template <typename Args, typename Types, std::size_t... I>
//bool HaveTypes(const Args& args, const Types& types, std::index_sequence<I...>) {
//    return (IsA(std::get<I>(args), std::get<I>(types)) && ...);
//}
//
//template <typename... Args, typename Types>
//bool HaveTypes(const std::tuple<Args...>& args, const Types& types) {
//    return HaveTypes(args, types, std::index_sequence_for<Args...>{});
//}
//
//template <typename... Args>
//bool ArgsAreTypes(const Args&... args) {
//    return (IsAType(args) && ...);
//}
//
//template <typename... Args>
//std::tuple<TypedVar_t<Args>...> MakeVars(const Args&... args) {
//    return std::make_tuple(var(args)...);
//}
//
//template <typename T, typename U>
//struct tuple_cat2;
//
//template <typename T, typename U>
//using tuple_cat2_t = typename tuple_cat2<T, U>::type;
//
//template <typename... Args1, typename... Args2>
//struct tuple_cat2<std::tuple<Args1...>, std::tuple<Args2...>> {
//    using type = std::tuple<Args1..., Args2...>;
//};
//
//template <typename... Args>
//struct tuple_cat;
//
//template <typename... Args>
//using tuple_cat_t = typename tuple_cat<Args...>::type;
//
//template <typename... Args>
//struct tuple_cat<std::tuple<Args...>> {
//    using type = std::tuple<Args...>;
//};
//
//template <typename... Args1, typename... Args2>
//struct tuple_cat<std::tuple<Args1...>, Args2...> {
//    using type = tuple_cat2_t<std::tuple<Args1...>, tuple_cat_t<Args2...>>;
//};
//
//template <typename... Args>
//struct Init;
//
//template <typename... Args>
//using init_t = typename Init<Args...>::type;
//
//template <typename Arg>
//struct Init<Arg> {
//    using type = std::tuple<>;
//};
//
//template <typename Arg, typename... Args>
//struct Init<Arg, Args...> {
//    using type = tuple_cat2_t<std::tuple<Arg>, init_t<Args...>>;
//};
//
//template <typename Arg, typename... Args>
//init_t<Arg, Args...> init(Arg&& arg, Args&&... args) {
//    return std::tuple_cat(std::tuple<Arg>{std::forward<Arg>(arg)}, init(std::forward<Args>(args)...));
//}
//
//template <typename Arg>
//init_t<Arg> init(Arg&&) {
//    return std::tuple<>{};
//}

//template <typename... Args>
//struct Last;
//
//template <typename... Args>
//using last_t = typename Last<Args...>::type;
//
//template <typename Arg>
//struct Last<Arg> {
//    using type = Arg;
//};
//
//template <typename Arg, typename... Args>
//struct Last<Arg, Args...> {
//    using type = last_t<Args...>;
//};
//
//template <typename Arg, typename... Args>
//last_t<Arg, Args...> last(Arg&&, Args&&... args) {
//    return last(std::forward<Args>(args)...);
//}
//
//template <typename Arg>
//last_t<Arg> last(Arg&& arg) {
//    return std::forward<Arg>(arg);
//}

//template <typename Arg, typename... Args>
//std::tuple<Args...> tail(Arg&&, Args&&... args) {
//    return {std::forward<Args>(args)...};
//}

//template <typename Arg, typename... Args>
//Arg head(Arg&& arg, Args&&...) {
//    return std::forward<Arg>(arg);
//}

//template <typename Arg>
//std::wostream& print_with_space(std::wostream& s, const Arg& arg) {
//    return s << ' ' << arg;
//}
//
//template <typename... Args>
//std::wostream& print_args(std::wostream& s, const Args&... args) {
//    (print_with_space(s, args), ...);
//    return s;
//}
//
//template <typename... Args>
//std::wostream& print_args(std::wostream& s, const std::tuple<Args...>& args) {
//    return std::apply([&s](const Args&... xs) -> std::wostream& { return print_args(s, xs...); }, args);
//}

