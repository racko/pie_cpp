#pragma once

#include "nat/add1.h"
#include "nat/nat.h"
#include "nat/zero.h"
#include "pie_base.h"
#include "strings.h"

inline CONSTEXPR auto one{define<One>(Nat, add1(zero))};
inline CONSTEXPR auto two{define<Two>(Nat, add1(one))};
inline CONSTEXPR auto three{define<Three>(Nat, add1(two))};
inline CONSTEXPR auto four{define<Four>(Nat, add1(three))};
inline CONSTEXPR auto five{define<Five>(Nat, add1(four))};
inline CONSTEXPR auto six{define<Six>(Nat, add1(five))};
inline CONSTEXPR auto seven{define<Seven>(Nat, add1(six))};
inline CONSTEXPR auto eight{define<Eight>(Nat, add1(seven))};
inline CONSTEXPR auto nine{define<Nine>(Nat, add1(eight))};
inline CONSTEXPR auto ten{define<Ten>(Nat, add1(nine))};
