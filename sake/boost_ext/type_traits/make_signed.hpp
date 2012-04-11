/*******************************************************************************
 * sake/boost_ext/type_traits/make_signed.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::make_signed<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_MAKE_SIGNED_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_MAKE_SIGNED_HPP

#include <boost/type_traits/make_signed.hpp>

#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>

namespace sake
{

namespace boost_ext
{

namespace make_signed_private
{

template< class T, bool = boost_ext::is_integral_or_enum<T>::value >
struct dispatch;

template< class T >
struct dispatch< T, true >
    : boost::make_signed<T>
{ };

template<>
struct dispatch< bool, true >
{ typedef bool type; };

template< class T >
struct dispatch< T, false >
{ typedef T type; };

} // namespace make_signed_private

template< class T >
struct make_signed
    : make_signed_private::dispatch<T>
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_MAKE_SIGNED_HPP
