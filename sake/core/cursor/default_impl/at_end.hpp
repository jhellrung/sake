/*******************************************************************************
 * sake/core/cursor/default_impl/at_end.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_DEFAULT_IMPL_AT_END_HPP
#define SAKE_CORE_CURSOR_DEFAULT_IMPL_AT_END_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/traits.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake_cursor_at_end_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME at_end
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class C >
inline bool
adl(C const & c)
{ return at_end(c); }

} // namespace sake_cursor_at_end_private

namespace sake
{

namespace cursor
{

namespace default_impl
{

namespace at_end_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME at_end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) bool ( )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class C >
inline bool
dispatch(C const & c, sake::int_tag<2>)
{ return c.at_end(); }

template< class C >
inline bool
dispatch(C const & c, sake::int_tag<1>)
{ return ::sake_cursor_at_end_private::adl(c); }

template< class C >
inline bool
dispatch(C const & c, sake::int_tag<0>)
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        sake::cursor_traits<C>::introversal,
        sake::end_access_introversal_tag
    >::value));
    return c == sake::cursor_traits<C>::end(c);
}

} // namespace at_end_private

template< class C >
inline bool
at_end(C const & c)
{
    typedef typename boost_ext::mpl::
         if_<
        at_end_private::is_callable_mem_fun< C const & >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_cursor_at_end_private::is_callable< bool ( C const & ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return at_end_private::dispatch(c, int_tag_());
}

} // namespace default_impl

} // namespace cursor

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_DEFAULT_IMPL_AT_END_HPP
