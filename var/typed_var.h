#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Type>
struct TypedVar_t : Pie<TypedVar_t<Type>> {
    constexpr TypedVar_t(const Type& type, const int id) : id_{id}, type_{type} {}

    int height_{};
    int id_;
    Type type_;
};

template <typename Type1, typename Type2>
constexpr bool equal(const TypedVar_t<Type1> lhs, const TypedVar_t<Type2> rhs) {
    return lhs.id_ == rhs.id_;
}

template <typename Type>
void print(std::ostream& s, const TypedVar_t<Type> x) {
    s << '[' << 'x' << x.id_ << ' ' << x.type_ << ']';
}

template <typename Type>
constexpr TypedVar_t<Type> var(const Type& type, const int id) {
    return TypedVar_t<Type>{type, id};
}

template <typename Type>
struct is_neutral<TypedVar_t<Type>> : std::true_type {};

template <typename Type>
struct synth_result<TypedVar_t<Type>> {
    using type = Type;
};

template <typename Type>
constexpr synth_result_t<TypedVar_t<Type>> synth1(const TypedVar_t<Type>& x) {
    assert(IsAType(x.type_));
    return x.type_;
}
