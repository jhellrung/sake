/*******************************************************************************
 * sake/boost_ext/type_traits/is_same_sans_cv.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_same_sans_cv<T,U>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_CV_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_CV_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T, class U >
struct is_same_sans_cv
    : boost::is_same<
          typename boost::remove_cv<T>::type,
          typename boost::remove_cv<U>::type
      >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_CV_HPP
