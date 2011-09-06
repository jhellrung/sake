/*******************************************************************************
 * boost_ext/type_traits/remove_qualifiers.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::remove_qualifiers<T>
 *
 * remove_qualifiers<T> -> T
 * remove_qualifiers< const T > -> remove_qualifiers<T>
 * remove_qualifiers< volatile T > -> remove_qualifiers<T>
 * remove_qualifiers< T& > -> remove_qualifiers<T>
 * remove_qualifiers< T&& > -> remove_qualifiers<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_QUALIFIERS_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_QUALIFIERS_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct remove_qualifiers { typedef T type; };
template< class T > struct remove_qualifiers< T const > : remove_qualifiers<T> { };
template< class T > struct remove_qualifiers< T volatile > : remove_qualifiers<T> { };
template< class T > struct remove_qualifiers< T const volatile > : remove_qualifiers<T> { };
template< class T > struct remove_qualifiers< T& > : remove_qualifiers<T> { };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct remove_qualifiers< T&& > : remove_qualifiers<T> { };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct remove_qualifiers< boost::rv<T> > : remove_qualifiers<T> { };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_QUALIFIERS_HPP
