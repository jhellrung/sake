/*******************************************************************************
 * sake/core/utility/int_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct int_tag<N>
 *
 * int_tag<T> is simply an empty struct.  It is intended to be used as a
 * function parameter for dispatching among several overloads.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_INT_TAG_HPP
#define SAKE_CORE_UTILITY_INT_TAG_HPP

namespace sake
{

template< int N >
struct int_tag
{
    static int const value = N;
    typedef int_tag type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_INT_TAG_HPP
