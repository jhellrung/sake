/*******************************************************************************
 * sake/boost_ext/type_traits/is_void.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_void<T>
 * struct boost_ext::not_is_void<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_VOID_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_VOID_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct is_void : boost::false_type { };
template<> struct is_void< void > : boost::true_type { };
template<> struct is_void< void const > : boost::true_type { };
template<> struct is_void< void volatile > : boost::true_type { };
template<> struct is_void< void const volatile > : boost::true_type { };

template< class T > struct not_is_void : boost::true_type { };
template<> struct not_is_void< void > : boost::false_type { };
template<> struct not_is_void< void const > : boost::false_type { };
template<> struct not_is_void< void volatile > : boost::false_type { };
template<> struct not_is_void< void const volatile > : boost::false_type { };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_VOID_HPP
