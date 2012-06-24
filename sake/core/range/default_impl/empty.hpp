/*******************************************************************************
 * sake/core/range/default_impl/empty.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_EMPTY_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_EMPTY_HPP

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/range/static_size.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake_range_empty_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_empty
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline bool
adl(R const & r)
{ return range_empty(r); }

} // namespace sake_range_empty_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace empty_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME empty
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) bool ( )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline bool
dispatch(R const & /*r*/, sake::int_tag<3>)
{ return sake::range_static_size<R>::value == 0; }

template< class R >
inline bool
dispatch(R const & r, sake::int_tag<2>)
{ return r.empty(); }

template< class R >
inline bool
dispatch(R const & r, sake::int_tag<1>)
{ return ::sake_range_empty_private::adl(r); }

template< class R >
inline bool
dispatch(R const & r, sake::int_tag<0>)
{
    typedef sake::range_traits<R> traits_;
    return traits_::begin(r) == traits_::end(r);
}

} // namespace empty_private

template< class R >
inline bool
empty(R const & r)
{
    typedef typename boost_ext::mpl::
         if_< sake::range_has_static_size<R>,
              sake::int_tag<3> >::type::template
    else_if < empty_private::is_callable_mem_fun< R const & >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_empty_private::is_callable< bool ( R const & ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return empty_private::dispatch(r, int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_EMPTY_HPP
