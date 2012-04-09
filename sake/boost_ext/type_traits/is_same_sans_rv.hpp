/*******************************************************************************
 * sake/boost_ext/type_traits/is_same_sans_rv.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::is_same_sans_rv<T,U>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_RV_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_RV_HPP

#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

namespace sake
{

namespace boost_ext
{

template< class T, class U >
struct is_same_sans_rv
    : boost::is_same<
          typename boost_ext::remove_rvalue_reference<T>::type,
          typename boost_ext::remove_rvalue_reference<U>::type
      >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_IS_SAME_SANS_RV_HPP
