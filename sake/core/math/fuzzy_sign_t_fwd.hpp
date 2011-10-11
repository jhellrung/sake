/*******************************************************************************
 * sake/core/math/fuzzy_sign_t_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_FUZZY_SIGN_T_FWD_HPP
#define SAKE_CORE_MATH_FUZZY_SIGN_T_FWD_HPP

#include <sake/core/math/indeterminate_fwd.hpp>
#include <sake/core/math/sign_t_fwd.hpp>
#include <sake/core/math/zero_fwd.hpp>

namespace sake
{

struct fuzzy_sign_t
{
    fuzzy_sign_t();
    fuzzy_sign_t(sake::zero_t);
    fuzzy_sign_t(sake::functional::indeterminate);
    fuzzy_sign_t(sake::zero_sign_t);
    fuzzy_sign_t(sake::positive_sign_t);
    fuzzy_sign_t(sake::negative_sign_t);
    fuzzy_sign_t(sake::indeterminate_sign_t);
    fuzzy_sign_t(sake::sign_t const s);
    explicit fuzzy_sign_t(int const s);

    fuzzy_sign_t& operator=(sake::zero_t);
    fuzzy_sign_t& operator=(sake::functional::indeterminate);
    fuzzy_sign_t& operator=(sake::zero_sign_t);
    fuzzy_sign_t& operator=(sake::positive_sign_t);
    fuzzy_sign_t& operator=(sake::negative_sign_t);
    fuzzy_sign_t& operator=(sake::indeterminate_sign_t);
    fuzzy_sign_t& operator=(sake::sign_t const s);

    fuzzy_sign_t operator-() const;

    fuzzy_sign_t& operator*=(fuzzy_sign_t const other);
    fuzzy_sign_t operator*(fuzzy_sign_t const other) const;

    fuzzy_sign_t sign() const;

    int as_int() const;
    int as_int_nothrow() const;

private:
    fuzzy_sign_t(unsigned int const value);
public:
    // public only to allow access to the free operators.
    static unsigned int const zero_sign_value = 0;
    static unsigned int const positive_sign_value = 1;
    static unsigned int const negative_sign_value = 3;
    static unsigned int const indeterminate_sign_value = 2;

    unsigned int m_value;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_FUZZY_SIGN_T_FWD_HPP
