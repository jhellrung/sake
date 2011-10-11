/*******************************************************************************
 * sake/core/math/sign_t.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct sign_t
 * ... const zero_sign
 * ... const positive_sign
 * ... const negative_sign
 * ... const indeterminate_sign
 * operator[op](sign_t, sign_t) -> bool
 * operator[op](sign_t, zero_t) -> bool
 * operator[op](zero_t, sign_t) -> bool
 *
 * [op] in { ==, !=, <, >, <=, >= }
 *
 * sign_t represents the sign (-1, 0, or +1) of a number.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIGN_T_HPP
#define SAKE_CORE_MATH_SIGN_T_HPP

#include <sake/core/math/fuzzy_sign_t_fwd.hpp>
#include <sake/core/math/sign_t_fwd.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>

namespace sake
{

#define define_comparison_operator( op ) \
inline bool operator op (sake::sign_t const s0, sake::sign_t const s1) \
{ return s0.as_int() op s1.as_int(); } \
inline bool operator op (sake::sign_t const s, sake::zero_t) \
{ return s.as_int() op 0; } \
inline bool operator op (sake::zero_t, sake::sign_t const s) \
{ return 0 op s.as_int(); }
define_comparison_operator( == )
define_comparison_operator( != )
define_comparison_operator( < )
define_comparison_operator( > )
define_comparison_operator( <= )
define_comparison_operator( >= )
#undef define_comparison_operator

inline
sign_t::
sign_t()
    : m_value(zero_value)
{ }

inline
sign_t::
sign_t(sake::zero_t)
    : m_value(zero_value)
{ }

inline
sign_t::
sign_t(sake::zero_sign_t)
    : m_value(zero_value)
{ }

inline
sign_t::
sign_t(sake::positive_sign_t)
    : m_value(positive_value)
{ }

inline
sign_t::
sign_t(sake::negative_sign_t)
    : m_value(negative_value)
{ }

inline
sign_t::
sign_t(sake::fuzzy_sign_t const s)
    : m_value(s.as_int())
{ }

inline
sign_t::
sign_t(int const s)
    : m_value(s)
{ SAKE_ASSERT_RELATION_ALL( (( -1, <=, s )) (( s, <=, +1 )) ); }

inline sign_t&
sign_t::
operator=(sake::zero_t)
{
    m_value = zero_value;
    return *this;
}

inline sign_t&
sign_t::
operator=(sake::zero_sign_t)
{
    m_value = zero_value;
    return *this;
}

inline sign_t&
sign_t::
operator=(sake::positive_sign_t)
{
    m_value = positive_value;
    return *this;
}

inline sign_t&
sign_t::
operator=(sake::negative_sign_t)
{
    m_value = negative_value;
    return *this;
}

inline sign_t
sign_t::
operator-() const
{ return sign_t(-m_value); }

inline sign_t&
sign_t::
operator*=(sign_t const other)
{
    m_value *= other.m_value;
    return *this;
}

inline sign_t
sign_t::
operator*(sign_t const other) const
{ return sign_t(m_value * other.m_value); }

inline sign_t
sign_t::
sign() const
{ return *this; }

inline int
sign_t::
as_int() const
{ return m_value; }

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIGN_T_HPP
