#pragma once

#include "pie_base.h"
#include "var.h"
#include <ostream>

template <typename Arg, typename Result>
struct Arrow_t : Pie<Arrow_t<Arg, Result>> {
    Arrow_t(const Arg& arg, const Result& result) : arg_{arg}, result_{result} {}

    Arg arg_;
    Result result_;
};

template <typename Arg1, typename Result1, typename Arg2, typename Result2>
bool operator==(const Arrow_t<Arg1, Result1>&, const Arrow_t<Arg2, Result2>&) {
    return false;
}

template <typename Arg, typename Result>
bool operator==(const Arrow_t<Arg, Result>& lhs, const Arrow_t<Arg, Result>& rhs) {
    return lhs.arg_ == rhs.arg_ && lhs.result_ == rhs.result_;
}

template <typename Arg, typename Result>
std::wostream& operator<<(std::wostream& s, const Arrow_t<Arg, Result>& type) {
    return s << "(-> " << type.arg_ << ' ' << type.result_ << ')';
}

template <typename Arg, typename Result>
Arrow_t<Arg, Result> Arrow(const Arg& arg, const Result& result) {
    return Arrow_t(arg, result);
}

template<typename F, typename Arg>
struct App_t : Pie<App_t<F, Arg>> {
    App_t(const F& f, const Arg& arg) : f_{f}, arg_{arg} {}

    F f_;
    Arg arg_;
};

template <typename F1, typename Arg1, typename F2, typename Arg2>
bool operator==(const App_t<F1, Arg1>&, const App_t<F2, Arg2>&) {
    return false;
}

template <typename F, typename Arg>
bool operator==(const App_t<F, Arg>& lhs, const App_t<F, Arg>& rhs) {
    return lhs.f_ == rhs.f_ && lhs.arg_ == rhs.arg_;
}

template <typename F, typename Arg>
std::wostream& operator<<(std::wostream& s, const App_t<F, Arg>& app) {
    return s << '(' << app.f_ << ' ' << app.arg_ << ')';
}

template <typename F>
struct Lambda_t : Pie<Lambda_t<F>> {
    Lambda_t(const F& f) : f_{f} {}

    template<typename Arg>
    App_t<Lambda_t<F>, Arg> operator()(const Arg& arg) const { return App_t<Lambda_t<F>, Arg>(*this, arg); }

    F f_;
};

template <typename F1, typename F2>
bool operator==(const Lambda_t<F1>&, const Lambda_t<F2>&) {
    return false;
}

template <typename F>
bool operator==(const Lambda_t<F>& lhs, const Lambda_t<F>& rhs) {
    static int n = 0;
    const int x = n++;
    return lhs.f_(x) == rhs.f_(x);
}

template <typename F>
std::wostream& operator<<(std::wostream& s, const Lambda_t<F>& f) {
    const Var_t var;
    return s << L"(λ  (" << var << ") " << f.f_(var) << ')';
}

template <typename F>
Lambda_t<F> lambda(const F& f) {
    return Lambda_t(f);
}

template <typename Arg, typename Result, typename F>
bool IsA_For_Values(const Lambda_t<F> f, Arrow_t<Arg, Result> type) {
    return IsA(f(the(type.arg_, var())), type.result_);
}

template <typename Arg, typename Result, typename F1, typename F2>
bool IsTheSameAs_For_Values(const Arrow_t<Arg, Result>& type, const Lambda_t<F1> lhs, const Lambda_t<F2> rhs) {
    const Var_t var;
    return IsTheSameAs(type.result_, lhs(the(type.arg_, var)), rhs(the(type.arg_, var)));
}

template <typename Arg, typename Result>
bool IsAType_For_Values(const Arrow_t<Arg, Result>& type) {
    return IsAType(type.arg_) && IsAType(type.result_);
}

template <typename Arg1, typename Result1, typename Arg2, typename Result2>
bool AreTheSameType_For_Values(const Arrow_t<Arg1, Result1>& lhs, const Arrow_t<Arg2, Result2>& rhs) {
    return AreTheSameType(lhs.arg_, rhs.arg_) && AreTheSameType(lhs.result_, rhs.result_);
}

//template <typename Arg, typename Result, typename F>
//bool IsANormal(const Lambda_t<F> value, Arrow_t<Arg, Result> type) { return IsA(atom, Atom); }

//bool IsNormalFormOfType(Arrow_t, Arrow_t) { return true; }

template <typename Arg, typename Result, typename F>
bool IsAValue(Arrow_t<Arg, Result> type, const Lambda_t<F> value) { return IsA(value, type); }

template <typename Arg, typename Result>
Arrow_t<Arg, Result> ComputeValue(const Arrow_t<Arg, Result>& type) { return type; }

template<typename F>
Lambda_t<F> ComputeValue(const Lambda_t<F> value) { return value; }

template<typename F, typename Arg>
auto ComputeValue(const App_t<F, Arg>& value) { return ComputeValue(value.f_).f_(value.arg_); }

//template <typename Arg, typename Result>
//auto Normalize(const Arrow_t<Arg, Result>& type) { return Arrow(Normalize(type.arg_), Normalize(type.result_)); }


//template<typename F>
//Lambda_t Normalize(const Lambda_t<F> value) { return value; }
