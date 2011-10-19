/*******************************************************************************
 * sake/core/math/inverse.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct inverse<T>
 *
 * Defines a simple wrapper class representing the (multiplicative) inverse of
 * a number.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERSE_HPP
#define SAKE_CORE_MATH_INVERSE_HPP

namespace sake
{

template< class T >
struct inverse
{
    typedef T value_type;

    explicit inverse(T const & value_)
        : m_value(value_)
    { }

    T const & value() const
    { return m_value; }

    template< class U >
    operator U() const
    { return static_cast<U>(1) / static_cast<U>(m_value); }

private:
    T m_value;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERSE_HPP
