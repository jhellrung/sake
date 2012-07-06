/*******************************************************************************
 * sake/core/iterator/is_convertible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_IS_CONVERTIBLE_HPP
#define SAKE_CORE_ITERATOR_IS_CONVERTIBLE_HPP

#include <cstddef>

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

namespace sake
{

namespace iterator
{

namespace is_convertible_private
{

template< class I, class J >
struct dispatch
    : boost::true_type
{ };

template< class T, class U >
struct dispatch< T*, U* >
    : boost_ext::is_same_sans_qualifiers<T,U>
{ };

template< class T, std::size_t N, class U >
struct dispatch< T[N], U* >
    : boost::false_type
{ };

} // namespace is_convertible_private

template< class I, class J >
struct is_convertible
    : boost_ext::mpl::and2<
          boost_ext::is_convertible<I,J>,
          is_convertible_private::dispatch<
              typename boost_ext::remove_qualifiers<I>::type,
              typename boost_ext::remove_qualifiers<J>::type
          >
      >
{ };

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_IS_CONVERTIBLE_HPP
