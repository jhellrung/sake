/*******************************************************************************
 * sake/core/range/adapt/traits/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_TRAITS_BASE_HPP
#define SAKE_CORE_RANGE_ADAPT_TRAITS_BASE_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/range/is_adapt_by_value.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

namespace traits
{

namespace base_private
{

template< class R >
class impl
{
    typedef typename boost_ext::remove_qualifiers<R>::type noqual_type;
    typedef typename boost::mpl::eval_if_c<
        sake::range_is_adapt_by_value< noqual_type >::value,
        boost::mpl::identity< noqual_type >,
        boost_ext::add_reference_add_const<R>
    >::type type;
};

} // namespace base_private

template< class R >
struct base
    : base_private::impl<
          typename boost_ext::remove_rvalue_reference<R>::type >
{ };

} // namespace traits

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_TRAITS_BASE_HPP
