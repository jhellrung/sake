/*******************************************************************************
 * sake/core/iterator/private/facade/difference_with_end_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_DIFFERENCE_WITH_END_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_DIFFERENCE_WITH_END_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class D, class P >
struct difference_with_end_enable
    : boost_ext::mpl::and2<
          boost_ext::is_convertible<
              typename D::cursor_introversal,
              sake::end_access_introversal_tag
          >,
          typename private_::traits<P>::difference_enable
      >
{ };

template< class D, class P >
struct difference_with_end_enabler
    : boost::enable_if_c<
          private_::difference_with_end_enable<D,P>::value,
          typename D::difference_type
      >
{ };

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_DIFFERENCE_WITH_END_ENABLE_HPP