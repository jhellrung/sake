/*******************************************************************************
 * sake/core/math/fuzzy_sign_t.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct fuzzy_sign_t
 * ... const fuzzy_sign_t::indeterminate
 * sruct indeterminate_sign_error // thrown in fuzzy_sign_t::as_int
 * operator[op](fuzzy_sign_t s0, fuzzy_sign_t s1) -> boost::logic::tribool
 * operator[op](fuzzy_sign_t s, zero_t) -> boost::logic::tribool
 * operator[op](zero_t, fuzzy_sign_t s) -> boost::logic::tribool
 * operator[op](fuzzy_sign_t s0, sign_t s1) -> boost::logic::tribool
 * operator[op](sign_t s0, fuzzy_sign_t s1) -> boost::logic::tribool
 *
 * [op] in { ==, !=, <, >, <=, >= }
 *
 * fuzzy_sign_t represents the "fuzzy" sign (-1, 0, +1, or indeterminate) of a
 * number.  This is useful to represent the sign of, for example, an approximate
 * scalar value, and is similar to Boost.Tribool.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_FUZZY_SIGN_T_HPP
#define SAKE_CORE_MATH_FUZZY_SIGN_T_HPP

#include <boost/logic/tribool.hpp>
#include <boost/throw_exception.hpp>

#include <sake/boost_ext/exception/define_exception.hpp>
#include <sake/core/math/fuzzy_sign_t_fwd.hpp>
#include <sake/core/math/indeterminate.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/assert.hpp>

namespace sake
{

SAKE_BOOST_EXT_DEFINE_EXCEPTION( sake, indeterminate_sign_error )

inline boost::logic::tribool
operator==(fuzzy_sign_t const s0, fuzzy_sign_t const s1)
{
    return sake::indeterminate(s0) || sake::indeterminate(s1) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.m_value == s1.m_value);
}

inline boost::logic::tribool
operator!=(fuzzy_sign_t const s0, fuzzy_sign_t const s1)
{
    return sake::indeterminate(s0) || sake::indeterminate(s1) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.m_value != s1.m_value);
}

#define define_operator( op ) \
inline boost::logic::tribool \
operator op (fuzzy_sign_t const s0, fuzzy_sign_t const s1) \
{ \
    return sake::indeterminate(s0) || sake::indeterminate(s1) ? \
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) : \
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() op s1.as_int_nothrow()); \
}
define_operator( < )
define_operator( > )
define_operator( <= )
define_operator( >= )
#undef define_operator

inline boost::logic::tribool
operator==(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value == fuzzy_sign_t::zero_sign_value);
}
inline boost::logic::tribool
operator!=(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value != fuzzy_sign_t::zero_sign_value);
}
inline boost::logic::tribool
operator<(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value == fuzzy_sign_t::negative_sign_value);
}
inline boost::logic::tribool
operator>(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value == fuzzy_sign_t::positive_sign_value);
}
inline boost::logic::tribool
operator<=(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value != fuzzy_sign_t::positive_sign_value);
}
inline boost::logic::tribool
operator>=(fuzzy_sign_t const s, sake::zero_t)
{
    return sake::indeterminate(s) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s.m_value != fuzzy_sign_t::negative_sign_value);
}

inline boost::logic::tribool
operator==(sake::zero_t, fuzzy_sign_t const s)
{ return operator==(s, sake::zero); }
inline boost::logic::tribool
operator!=(sake::zero_t, fuzzy_sign_t const s)
{ return operator!=(s, sake::zero); }
inline boost::logic::tribool
operator<(sake::zero_t, fuzzy_sign_t const s)
{ return operator>(s, sake::zero); }
inline boost::logic::tribool
operator>(sake::zero_t, fuzzy_sign_t const s)
{ return operator<(s, sake::zero); }
inline boost::logic::tribool
operator<=(sake::zero_t, fuzzy_sign_t const s)
{ return operator>=(s, sake::zero); }
inline boost::logic::tribool
operator>=(sake::zero_t, fuzzy_sign_t const s)
{ return operator<=(s, sake::zero); }

inline boost::logic::tribool
operator==(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() == s1.as_int());
}
inline boost::logic::tribool
operator!=(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() != s1.as_int());
}
inline boost::logic::tribool
operator<(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() < s1.as_int());
}
inline boost::logic::tribool
operator>(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() > s1.as_int());
}
inline boost::logic::tribool
operator<=(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() <= s1.as_int());
}
inline boost::logic::tribool
operator>=(fuzzy_sign_t const s0, sake::sign_t const s1)
{
    return sake::indeterminate(s0) ?
           static_cast< boost::logic::tribool >(boost::logic::indeterminate) :
           static_cast< boost::logic::tribool >(s0.as_int_nothrow() >= s1.as_int());
}

inline boost::logic::tribool
operator==(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator==(s1, s0); }
inline boost::logic::tribool
operator!=(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator!=(s1, s0); }
inline boost::logic::tribool
operator<(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator>(s1, s0); }
inline boost::logic::tribool
operator>(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator<(s1, s0); }
inline boost::logic::tribool
operator<=(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator>=(s1, s0); }
inline boost::logic::tribool
operator>=(sake::sign_t const s0, fuzzy_sign_t const s1)
{ return operator<=(s1, s0); }


inline
fuzzy_sign_t::
fuzzy_sign_t()
    : m_value(indeterminate_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::zero_t)
    : m_value(zero_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::functional::indeterminate)
    : m_value(indeterminate_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::sign_t::zero_tag)
    : m_value(zero_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::sign_t::positive_tag)
    : m_value(positive_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::sign_t::negative_tag)
    : m_value(negative_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::fuzzy_sign_t::indeterminate_tag)
    : m_value(indeterminate_sign_value)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(sake::sign_t const s)
    : m_value(static_cast< unsigned int >(s.as_int() + 4) & 3)
{ }

inline
fuzzy_sign_t::
fuzzy_sign_t(int const s)
    : m_value(static_cast< unsigned int >(s + 4) & 3)
{ SAKE_ASSERT_RELATION_ALL( (( -1, <=, s )) (( s, <=, +1 )) ); }

inline
fuzzy_sign_t::
fuzzy_sign_t(unsigned int const value)
    : m_value(value)
{ }

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::zero_t)
{
    m_value = zero_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::functional::indeterminate)
{
    m_value = indeterminate_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::sign_t::zero_tag)
{
    m_value = zero_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::sign_t::positive_tag)
{
    m_value = positive_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::sign_t::negative_tag)
{
    m_value = negative_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::fuzzy_sign_t::indeterminate_tag)
{
    m_value = indeterminate_sign_value;
    return *this;
}

inline fuzzy_sign_t&
fuzzy_sign_t::
operator=(sake::sign_t const s)
{
    m_value = static_cast< unsigned int >(s.m_value + 4) & 3;
    return *this;
}

inline fuzzy_sign_t
fuzzy_sign_t::
operator-() const
{ return fuzzy_sign_t((4 - m_value) & 3); }

inline fuzzy_sign_t&
fuzzy_sign_t::
operator*=(fuzzy_sign_t const other)
{
    unsigned int const x = m_value * other.m_value;
    if(x != 4)
        m_value = x & 3;
    return *this;
}

inline fuzzy_sign_t
fuzzy_sign_t::
operator*(fuzzy_sign_t const other) const
{
    unsigned int const x = m_value * other.m_value;
    return fuzzy_sign_t(x == 4 ? indeterminate_sign_value : x & 3);
}

inline fuzzy_sign_t
fuzzy_sign_t::
sign() const
{ return *this; }

inline int
fuzzy_sign_t::
as_int() const
{
    if(m_value == indeterminate_sign_value)
        boost::throw_exception(indeterminate_sign_error());
    return as_int_nothrow();
}

inline int
fuzzy_sign_t::
as_int_nothrow() const
{
    SAKE_ASSERT_RELATION( m_value, !=, static_cast< unsigned int >(indeterminate_sign_value) );
    return static_cast< int >((m_value + 1) & 3) - 1;
}

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_FUZZY_SIGN_T_HPP
