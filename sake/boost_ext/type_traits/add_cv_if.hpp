/*******************************************************************************
 * boost_ext/type_traits/add_cv_if.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_cv_if< AddConst, AddVolatile, T >
 * struct boost_ext::add_cv_if_c< AddConst, AddVolatile, T >
 *
 * This conditionally adds const and volatile qualifiers to T.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CV_IF_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CV_IF_HPP

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_cv.hpp>
#include <boost/type_traits/add_volatile.hpp>

namespace sake
{

namespace boost_ext
{

template< bool AddConst, bool AddVolatile, class T >
struct add_cv_if_c;

template< class T > struct add_cv_if_c< false, false, T > { typedef T type; };
template< class T > struct add_cv_if_c< false,  true, T > : boost::add_volatile<T> { };
template< class T > struct add_cv_if_c<  true, false, T > : boost::add_const<T> { };
template< class T > struct add_cv_if_c<  true,  true, T > : boost::add_cv<T> { };

template< class AddConst, class AddVolatile, class T >
struct add_cv_if
    : add_cv_if_c< AddConst::value, AddVolatile::value, T >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CV_IF_HPP
