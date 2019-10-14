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
struct Printer<TypedVar_t<Type, id>> {
    static void print(std::ostream& s) { s << '[' << 'x' << id << ' ' << Print<Type>{} << ']'; }
};

template <int id, typename Type>
constexpr TypedVar_t<Type, id> var(const Type& type) {
    return TypedVar_t<Type, id>{type};
}

template <typename Type, int id>
struct is_neutral<TypedVar_t<Type, id>> : std::true_type {};

template <typename Type, int id>
struct Synth<TypedVar_t<Type, id>> {
    static_assert(is_a_type_v<Type>);
    using type = Type;
};
