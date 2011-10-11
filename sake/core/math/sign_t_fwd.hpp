/*******************************************************************************
 * sake/core/math/sign_t_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIGN_T_FWD_HPP
#define SAKE_CORE_MATH_SIGN_T_FWD_HPP

#include <sake/core/math/zero_fwd.hpp>

namespace sake
{

struct fuzzy_sign_t;

struct zero_sign_t { };
struct positive_sign_t { };
struct negative_sign_t { };
struct indeterminate_sign_t { };
zero_sign_t const zero_sign = { };
positive_sign_t const positive_sign = { };
negative_sign_t const negative_sign = { };
indeterminate_sign_t const indeterminate_sign = { };

struct sign_t
{
    sign_t();
    sign_t(zero_t);
    sign_t(zero_sign_t);
    sign_t(positive_sign_t);
    sign_t(negative_sign_t);
    explicit sign_t(sake::fuzzy_sign_t const s);
    explicit sign_t(int const s);

    sign_t& operator=(zero_t);
    sign_t& operator=(zero_sign_t);
    sign_t& operator=(positive_sign_t);
    sign_t& operator=(negative_sign_t);

    sign_t operator-() const;
    sign_t& operator*=(sign_t const other);
    sign_t operator*(sign_t const other) const;

    sign_t sign() const;

    int as_int() const;

private:
    friend struct sake::fuzzy_sign_t;

    static int const zero_value = 0;
    static int const positive_value = +1;
    static int const negative_value = -1;

    int m_value;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIGN_T_FWD_HPP
