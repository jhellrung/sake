/*******************************************************************************
 * boost_ext/type_traits/is_rvalue_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_rvalue_reference<T>
 *
 * is_rvalue_reference<T> -> false
 * is_rvalue_reference< T&& > -> true
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct is_rvalue_reference : boost::false_type { };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct is_rvalue_reference< T&& > : boost::true_type { };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct is_rvalue_reference< boost::rv<T> & > : boost::true_type { };
template< class T > struct is_rvalue_reference< boost::rv<T> const & > : boost::true_type { };
template< class T > struct is_rvalue_reference< boost::rv<T> volatile & > : boost::true_type { };
template< class T > struct is_rvalue_reference< boost::rv<T> const volatile & > : boost::true_type { };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP
