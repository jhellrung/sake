/*******************************************************************************
 * core/utility/void.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct void_
 * struct is_void_<T>
 * struct is_not_void_<T>
 *
 * void_ is simply an empty dummy struct.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_VOID_HPP
#define SAKE_CORE_UTILITY_VOID_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

struct void_ { };

template< class T >
struct is_void_
    : boost::false_type
{ };
template<>
struct is_void_< void_ >
    : boost::true_type
{ };

template< class T >
struct is_not_void_
    : boost::true_type
{ };
template<>
struct is_not_void_< void_ >
    : boost::false_type
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_VOID_HPP
