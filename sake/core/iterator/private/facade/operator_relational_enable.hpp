/*******************************************************************************
 * sake/core/iterator/private/facade/operator_relational_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_RELATIONAL_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_RELATIONAL_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/iterator/private/is_interconvertible.hpp>
#include <sake/core/math/sign_t.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class D0, class P0, class D1, class P1 >
struct operator_relational_enable
    : boost_ext::mpl::and3<
          typename private_::traits< P0 >::operator_relational_enable,
          typename private_::traits< P1 >::operator_relational_enable,
          sake::iterator_private::is_interconvertible< D0, D1 >
      >
{ };

template< class D0, class P0, class D1, class P1 >
struct operator_relational_enabler
    : boost::enable_if_c<
          private_::operator_relational_enable< D0, P0, D1, P1 >::value,
          bool
      >
{ };

template< class D0, class P0, class D1, class P1 >
struct cmp_enabler
    : boost::enable_if_c<
          private_::operator_relational_enable< D0, P0, D1, P1 >::value,
          sake::sign_t
      >
{ };

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_RELATIONAL_ENABLE_HPP
