#pragma once

#include "pie_base.h"
#include <ostream>

template <typename Type, int id>
struct TypedVar_t : Pie<TypedVar_t<Type, id>> {
    constexpr TypedVar_t(const Type& type) : type_{type} {}

    Type type_;
};

template <typename Type, int id>
struct Height<TypedVar_t<Type, id>> : std::integral_constant<int, 0> {};

template <typename Type1, int id1, typename Type2, int id2>
struct Equal<TypedVar_t<Type1, id1>, TypedVar_t<Type2, id2>> : std::bool_constant<id1 == id2> {};

template <typename Type, int id>
void print(std::ostream& s, const TypedVar_t<Type, id> x) {
    s << '[' << 'x' << id << ' ' << x.type_ << ']';
}

template <int id, typename Type>
constexpr TypedVar_t<Type, id> var(const Type& type) {
    return TypedVar_t<Type, id>{type};
}

template <typename Type, int id>
struct is_neutral<TypedVar_t<Type, id>> : std::true_type {};

template <typename Type, int id>
struct synth_result<TypedVar_t<Type, id>> {
    using type = Type;
};

template <typename Type, int id>
constexpr synth_result_t<TypedVar_t<Type, id>> synth1(const TypedVar_t<Type, id>& x) {
    assert(IsAType(x.type_));
    return x.type_;
}
