/*******************************************************************************
 * sake/core/utility/default_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct default_tag
 * struct is_default_tag<T>
 * struct is_not_default_tag<T>
 * struct replace_default_tag<T,U>
 * struct lazy_replace_default_tag<T,U>
 * default_tag const default_
 *
 * A class template can interpret an instantiation with default_tag as a
 * template parameter to set that parameter to some default type that would
 * otherwise be cumbersome or too implementation-specific to express.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFAULT_TAG_HPP
#define SAKE_CORE_UTILITY_DEFAULT_TAG_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

struct default_tag
{ };

default_tag const default_ = { };

template< class T >
struct is_default_tag
    : boost::false_type
{ };
template<>
struct is_default_tag< default_tag >
    : boost::true_type
{ };

template< class T >
struct is_not_default_tag
    : boost::true_type
{ };
template<>
struct is_not_default_tag< default_tag >
    : boost::false_type
{ };

template< class T, class U >
struct replace_default_tag
{ typedef T type; };
template< class U >
struct replace_default_tag< default_tag, U >
{ typedef U type; };

template< class T, class U >
struct lazy_replace_default_tag
{ typedef T type; };
template< class U >
struct lazy_replace_default_tag< default_tag, U >
{ typedef typename U::type type; };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_DEFAULT_TAG_HPP
