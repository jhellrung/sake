/*******************************************************************************
 * sake/core/utility/type_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct type_tag<T>
 * struct is_type_tag<T>
 * struct remove_type_tag<T>
 *
 * type_tag<T> is simply an empty struct with a single "typedef T type;".  It
 * is intended to be used as a function parameter to allow a type template
 * parameter to be deduced rather than explicitly given.  This would be
 * necessary when one wants ADL to apply to the function call.  For example,
 * compare:
 *
 * template< class T > void f();
 * template< class T > void g(type_tag<T>);
 *
 * f<T>()
 * g(type_tag<T>())
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_TYPE_TAG_HPP
#define SAKE_CORE_UTILITY_TYPE_TAG_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

template< class T >
struct type_tag
{ typedef T type; };

template< class T >
struct is_type_tag
    : boost::false_type
{ };

template< class T >
struct is_type_tag< type_tag<T> >
    : boost::true_type
{ };

template< class T >
struct remove_type_tag
{ typedef T type; };

template< class T >
struct remove_type_tag< type_tag<T> >
{ typedef T type; };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_TYPE_TAG_HPP
