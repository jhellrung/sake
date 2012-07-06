/*******************************************************************************
 * sake/core/iterator/private/facade/at_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_AT_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_AT_ENABLE_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/is_convertible_relax.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

namespace private_
{

template< class D, class T >
struct at_enable
    : boost_ext::mpl::or3<
          boost_ext::mpl::and2<
              boost::is_same< T, sake::begin_tag >,
              boost_ext::is_convertible<
                  typename D::iterator_introversal,
                  sake::begin_access_introversal_tag
              >
          >,
          boost_ext::mpl::and2<
              boost::is_same< T, sake::end_tag >,
              boost_ext::is_convertible<
                  typename D::iterator_introversal,
                  sake::end_access_introversal_tag
              >
          >,
          sake::iterator::private_::is_convertible_relax<T,D>
      >
{ };

template< class D, class T, class R = void >
struct at_enabler
    : boost::enable_if_c< private_::at_enable<D,T>::value, R >
{ };

template< class D, class T, class R >
struct at_lazy_enabler
    : boost::lazy_enable_if_c< private_::at_enable<D,T>::value, R >
{ };

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_AT_ENABLE_HPP
