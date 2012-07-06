/*******************************************************************************
 * sake/core/range/private/facade/sub_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SUB_ENABLE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_SUB_ENABLE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/range/private/facade/iter_at_enable.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class P, class B, class E >
struct sub_enable
    : boost_ext::mpl::and2<
          private_::iter_at_enable<P,B>,
          private_::iter_at_enable<P,E>
      >
{ };

template< class P, class B, class E, class R = void >
struct sub_enabler
    : boost::enable_if_c< private_::sub_enable<P,B,E>::value, R >
{ };

template< class P, class B, class E, class R >
struct sub_lazy_enabler
    : boost::lazy_enable_if_c< private_::sub_enable<P,B,E>::value, R >
{ };

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_SUB_ENABLE_HPP
