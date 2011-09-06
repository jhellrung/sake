/*******************************************************************************
 * boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_lvalue_reference_to_nonconst<T>
 *
 * is_lvalue_reference_to_nonconst<T> -> false
 * is_lvalue_reference_to_nonconst< T& > -> true
 * is_lvalue_reference_to_nonconst< const T& > -> false
 * is_lvalue_reference_to_nonconst< T&& > -> false
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_LVALUE_REFERENCE_TO_NONCONST_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_LVALUE_REFERENCE_TO_NONCONST_HPP

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct is_lvalue_reference_to_nonconst : boost::false_type { };
template< class T > struct is_lvalue_reference_to_nonconst< T & > : boost::true_type { };
template< class T > struct is_lvalue_reference_to_nonconst< T const & > : boost::false_type { };
#ifdef BOOST_NO_RVALUE_REFERENCES
template< class T > struct is_lvalue_reference_to_nonconst< boost::rv<T> & > : boost::false_type { };
template< class T > struct is_lvalue_reference_to_nonconst< boost::rv<T> const & > : boost::false_type { };
template< class T > struct is_lvalue_reference_to_nonconst< boost::rv<T> volatile & > : boost::false_type { };
template< class T > struct is_lvalue_reference_to_nonconst< boost::rv<T> const volatile & > : boost::false_type { };
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_LVALUE_REFERENCE_TO_NONCONST_HPP
