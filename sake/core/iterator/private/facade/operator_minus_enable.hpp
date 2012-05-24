/*******************************************************************************
 * sake/core/iterator/private/facade/operator_minus_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_MINUS_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_MINUS_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/iterator/private/common_difference_type.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/iterator/private/is_interconvertible.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class D0, class P0, class D1, class P1 >
struct operator_minus_enable
    : boost_ext::mpl::and3<
          typename private_::traits< P0 >::operator_minus_enable,
          typename private_::traits< P1 >::operator_minus_enable,
          sake::iterator_private::is_interconvertible< D0, D1 >
      >
{ };

template< class D0, class P0, class D1, class P1 >
struct operator_minus_enabler
    : boost::lazy_enable_if_c<
          private_::operator_minus_enable< D0, P0, D1, P1 >::value,
          sake::iterator_private::common_difference_type< D0, D1 >
      >
{ };

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_MINUS_ENABLE_HPP
