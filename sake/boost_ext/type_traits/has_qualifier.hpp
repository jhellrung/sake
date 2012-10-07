/*******************************************************************************
 * sake/boost_ext/type_traits/has_qualifier.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::has_qualifier<T>
 *
 * has_qualifier<T> -> false
 * has_qualifier< const T > -> true
 * has_qualifier< volatile T > -> true
 * has_qualifier< T& > -> true
 * has_qualifier< T&& > -> true
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_HAS_QUALIFIER_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_HAS_QUALIFIER_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct has_qualifier : boost::false_type { };
template< class T > struct has_qualifier< T const > : boost::true_type { };
template< class T > struct has_qualifier< T volatile > : boost::true_type { };
template< class T > struct has_qualifier< T const volatile > : boost::true_type { };
template< class T > struct has_qualifier< T& > : boost::true_type { };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct has_qualifier< T&& > : boost::true_type { };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct has_qualifier< boost::rv<T> > : boost::true_type { };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_HAS_QUALIFIER_HPP
