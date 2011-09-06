/*******************************************************************************
 * boost_ext/type_traits/is_cv_and.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_cv_and<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CV_AND_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_CV_AND_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct is_cv_and : boost::false_type { };
template< class T > struct is_cv_and< T const > : boost::false_type { };
template< class T > struct is_cv_and< T volatile > : boost::false_type { };
template< class T > struct is_cv_and< T const volatile > : boost::true_type { };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_CV_AND_HPP
