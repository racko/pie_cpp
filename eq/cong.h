#pragma once

#include "eq/eq.h"
#include "eq/same.h"
#include "pie_base.h"
#include <ostream>
#include <type_traits>
#include <utility>

template <typename Target, typename F>
struct Cong_t : Pie<Cong_t<Target, F>> {
    constexpr Cong_t(Target target, F f) : target_{std::move(target)}, f_{std::move(f)} {}

    Target target_;
    F f_;
};

template <typename Target1, typename F1, typename Target2, typename F2>
constexpr bool equal(const Cong_t<Target1, F1>& lhs, const Cong_t<Target2, F2>& rhs, int& next_index) {
    return equal(lhs.target_, rhs.target_, next_index) && equal(lhs.f_, rhs.f_, next_index);
}

template <typename Target, typename F>
void print(std::ostream& s, const Cong_t<Target, F>& cong, int& next_index) {
    s << "(cong " << InContext(cong.target_, next_index) << ' ' << InContext(cong.f_, next_index) << ')';
}

template <typename Target, typename F>
constexpr Cong_t<Target, F> cong(const Pie<Target>& target, const Pie<F>& f) {
    return Cong_t<Target, F>(target.derived(), f.derived());
}

template <typename Target, typename F>
struct step_result<Cong_t<Target, F>> {
    using type = Same_t<apply_result_t<F, typename compute_value_result_t<Target>::type>>;
};

template <typename Target, typename F>
constexpr step_result_t<Cong_t<Target, F>> Step(const Cong_t<Target, F>& cong) {
    assert(!is_neutral_v<Target>);
    return same(cong.f_(ComputeValue(cong.target_).x_));
}

template <typename Target, typename F>
struct is_neutral<Cong_t<Target, F>> : std::bool_constant<is_neutral_v<Target>> {};

template <typename Target, typename F>
struct synth_result<Cong_t<Target, F>> {
  private:
    using target_type = synth_result_t<Target>;

  public:
    using type = Eq_t<std::invoke_result_t<typename synth_result_t<F>::result_type, typename target_type::from_t>,
                      apply_result_t<F, typename target_type::from_t>,
                      apply_result_t<F, typename target_type::to_t>>;
};

template <typename Target, typename F>
constexpr synth_result_t<Cong_t<Target, F>> synth1(const Cong_t<Target, F>& cong, int& next_index) {
    const auto target_type = synth1(cong.target_, next_index);
    const auto f_type = synth1(cong.f_, next_index);
    assert(AreTheSameType1(target_type.type_, f_type.arg_, next_index));
    return Eq(f_type.result_(target_type.from_), cong.f_(target_type.from_), cong.f_(target_type.to_));
}
