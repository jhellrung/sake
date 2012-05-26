/*******************************************************************************
 * sake/core/iterator/private/facade/equal_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_EQUAL_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_EQUAL_ENABLE_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/private/is_interconvertible.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class D0, class P0, class D1, class P1 >
struct equal_enable
    : boost_ext::mpl::and2<
          boost_ext::is_convertible<
              typename sake::iterator_traversal< D0 >::type,
              boost::single_pass_traversal_tag
          >,
          /*boost_ext::is_convertible<
              typename sake::iterator_traversal< D1 >::type,
              boost::single_pass_traversal_tag
          >,*/
          sake::iterator_private::is_interconvertible< D0, D1 >
      >
{ };

template< class D0, class P0, class D1, class P1 >
struct equal_enabler
    : boost::enable_if_c<
          private_::equal_enable< D0, P0, D1, P1 >::value,
          bool
      >
{ };

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_EQUAL_ENABLE_HPP
