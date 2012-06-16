/*******************************************************************************
 * sake/core/range/default_impl/distance.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/distance.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake_range_distance_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_distance
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline typename ::sake::range_traits<R>::difference_type
adl(R const & r)
{ return range_distance(r); }

} // namespace sake_range_distance_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace distance_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME size
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline typename sake::range_traits<R>::difference_type
dispatch(R const & r, sake::int_tag<2>)
{ return r.distance(); }

template< class R >
inline typename sake::range_traits<R>::difference_type
dispatch(R const & r, sake::int_tag<1>)
{ return ::sake_range_distance_private::adl(r); }

template< class R >
inline typename sake::range_traits<R>::difference_type
dispatch(R const & r, sake::int_tag<0>)
{
    return sake::iterator::distance(
        sake::range_traits<R>::begin(r, sake::end_access_introversal_tag()),
        sake::_end
    );
}

} // namespace distance_private

template< class R >
inline typename sake::range_traits<R>::difference_type
distance(R const & r)
{
    SAKE_USING_TYPEDEF( typename sake::range_traits<R>, difference_type );
    typedef typename boost_ext::mpl::
    if_<
        distance_private::is_callable_mem_fun<
            R const &, difference_type ( ) >,
        sake::int_tag<2>
    >::type::template
    else_if<
        ::sake_range_distance_private::is_callable<
            difference_type ( R const & ) >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type int_tag_;
    return distance_private::dispatch(r, int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_DISTANCE_HPP
