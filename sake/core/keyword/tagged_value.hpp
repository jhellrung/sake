/*******************************************************************************
 * sake/core/keyword/tagged_value.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_TAGGED_VALUE_HPP
#define SAKE_CORE_KEYWORD_TAGGED_VALUE_HPP

#include <boost/static_assert.hpp>

#include <sake/core/utility/is_by_value_optimal.hpp>

namespace sake
{

namespace keyword
{

template< class Tag, class T >
struct tagged_value
{
    BOOST_STATIC_ASSERT((sake::is_by_value_optimal<T>::value));

    typedef Tag tag;
    typedef T value_type;

    // implicit on purpose
    tagged_value(T value_) : m_value(value_) { }

    T value() const
    { return static_cast<T>(m_value); }

private:
    T m_value;
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_TAGGED_VALUE_HPP
