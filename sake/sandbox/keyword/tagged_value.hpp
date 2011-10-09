/*******************************************************************************
 * sake/sandbox/keyword/tagged_value.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_SANDBOX_KEYWORD_TAGGED_VALUE_HPP
#define SAKE_SANDBOX_KEYWORD_TAGGED_VALUE_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_reference.hpp>

namespace sake
{

namespace keyword
{

template< class Tag, class T >
struct tagged_value
{
    BOOST_STATIC_ASSERT((boost_ext::is_reference<T>::value));
    typedef Tag tag;
    typedef T value_type;
    tagged_value(T value)
        : m_value(value)
    { }
    T value() const
    { return static_cast<T>(m_value); }
private:
    T m_value;
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_SANDBOX_KEYWORD_TAGGED_VALUE_HPP
