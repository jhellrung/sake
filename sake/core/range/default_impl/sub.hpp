/*******************************************************************************
 * sake/core/range/default_impl/sub.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_SUB_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_SUB_HPP

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake_range_sub_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_sub
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 3, 3 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R, class Begin, class End >
inline typename ::sake::range_subrange< R, Begin, End >::type
adl(R& r, Begin const & b, End const & e)
{ return range_sub(r,b,e); }

} // namespace sake_range_sub_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace sub_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME sub
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R, class Begin, class End >
inline typename sake::range_subrange< R, Begin, End >::type
dispatch(R& r, Begin const & b, End const & e, sake::int_tag<2>)
{ return r.sub(b,e); }

template< class R, class Begin, class End >
inline typename sake::range_subrange< R, Begin, End >::type
dispatch(R& r, Begin const & b, End const & e, sake::int_tag<1>)
{ return ::sake_range_sub_private::adl(r,b,e); }

template< class R, class Begin, class End >
inline typename sake::range_subrange< R, Begin, End >::type
dispatch(R& r, Begin const & b, End const & e, sake::int_tag<0>)
{ return typename sake::range_subrange< R, Begin, End >::type(r,b,e); }

} // namespace sub_private

template< class R, class Begin, class End >
inline typename sake::range_subrange< R, Begin, End >::type
sub(R& r, Begin const & b, End const & e)
{
    typedef typename sake::range_subrange< R, Begin, End >::type result_type;
    typedef typename boost_ext::mpl::
         if_< sub_private::is_callable_mem_fun<
                  R&, result_type ( Begin, End ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_sub_private::is_callable<
                  result_type ( R&, Begin, End ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return sub_private::dispatch(r, b, e, int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITER_AT_HPP
