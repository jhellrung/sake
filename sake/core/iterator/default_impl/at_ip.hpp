/*******************************************************************************
 * sake/core/iterator/default_impl/at_ip.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_IP_HPP
#define SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_IP_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/is_interoperable.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake_iterator_at_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_at_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I, class T >
inline void
adl(I& i, T const & x)
{ iterator_at_ip(i,x); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_begin
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_begin_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I >
inline void
adl_begin(I& i)
{ iterator_begin_ip(i); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_end
#define SAKE_INTROSPECTION_FUNCTION_NAME iterator_end_ip
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class I >
inline void
adl_end(I& i)
{ iterator_end_ip(i); }

} // namespace sake_iterator_at_ip_private

namespace sake
{

namespace iterator
{

namespace default_impl
{

namespace at_ip_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME at_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I, class T >
inline void
dispatch(I& i, T const & x, sake::int_tag<2>)
{ i.at_ip(x); }

template< class I, class T >
inline void
dispatch(I& i, T const & x, sake::int_tag<1>)
{ ::sake_iterator_at_ip_private::adl(i,x); }

template< class I, class T >
inline void
dispatch(I& i, T const & x, sake::int_tag<0>)
{ i = x; }

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_begin
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME begin_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I >
inline void
dispatch_begin(I& i, sake::int_tag<2>)
{ i.begin_ip(); }

template< class I >
inline void
dispatch_begin(I& i, sake::int_tag<1>)
{ ::sake_iterator_at_ip_private::adl_begin(i); }

template< class I >
inline void
dispatch_begin(I& i, sake::int_tag<0>)
{ i = sake::_begin; }

template< class I >
inline void
dispatch(I& i, sake::begin_tag, sake::int_tag<0>)
{
    typedef typename boost_ext::mpl::
         if_< at_ip_private::is_callable_mem_fun_begin< I& >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_ip_private::is_callable_begin< void ( I& ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_begin(i, int_tag_());
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_end
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME end_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class I >
inline void
dispatch_end(I& i, sake::int_tag<2>)
{ i.end_ip(); }

template< class I >
inline void
dispatch_end(I& i, sake::int_tag<1>)
{ ::sake_iterator_at_ip_private::adl_end(i); }

template< class I >
inline void
dispatch_end(I& i, sake::int_tag<0>)
{ i = sake::_end; }

template< class I >
inline void
dispatch(I& i, sake::end_tag, sake::int_tag<0>)
{
    typedef typename boost_ext::mpl::
         if_< at_ip_private::is_callable_mem_fun_end< I& >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_ip_private::is_callable_end< void ( I& ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_end(i, int_tag_());
}

} // namespace at_ip_private

template< class I, class T >
inline void
at_ip(I& i, T const & x)
{
    typedef typename sake::iterator_introversal<I>::type introversal;
    BOOST_STATIC_ASSERT((boost_ext::mpl::or3<
        boost_ext::mpl::and2<
            boost::is_same< T, sake::begin_tag >,
            boost_ext::is_convertible<
                introversal, sake::begin_access_introversal_tag >
        >,
        boost_ext::mpl::and2<
            boost::is_same< T, sake::end_tag >,
            boost_ext::is_convertible<
                introversal, sake::end_access_introversal_tag >
        >,
        sake::iterator::private_::is_interoperable<I,T>
    >::value));
    typedef typename boost_ext::mpl::
         if_< at_ip_private::is_callable_mem_fun< I&, void ( T ) >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_iterator_at_ip_private::is_callable< void ( I&, T ) >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    at_ip_private::dispatch(i, x, int_tag_());
}

} // namespace default_impl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_AT_IP_HPP
