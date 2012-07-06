/*******************************************************************************
 * sake/core/range/default_impl/at.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_AT_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_AT_HPP

#include <boost/mpl/placeholders.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/introspection/has_operator_bracket.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/math/prior.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake_range_at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME range_at
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 2, 2 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R, class T >
inline typename ::sake::range_reference<R>::type
adl(R& r, T const x)
{ return range_at(r,x); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_front
#define SAKE_INTROSPECTION_FUNCTION_NAME range_front
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline typename ::sake::range_reference<R>::type
adl_front(R& r)
{ return range_front(r); }

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable_back
#define SAKE_INTROSPECTION_FUNCTION_NAME range_back
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class R >
inline typename ::sake::range_reference<R>::type
adl_back(R& r)
{ return range_back(r); }

} // namespace sake_range_at_private

namespace sake
{

namespace range
{

namespace default_impl
{

namespace at_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME at
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R, class T >
inline typename sake::range_reference<R>::type
dispatch(R& r, T const x, sake::int_tag<3>)
{ return r[x]; }

template< class R, class T >
inline typename sake::range_reference<R>::type
dispatch(R& r, T const x, sake::int_tag<2>)
{ return r.at(x); }

template< class R, class T >
inline typename sake::range_reference<R>::type
dispatch(R& r, T const x, sake::int_tag<1>)
{ return ::sake_range_at_private::adl(r,x); }

template< class R, class T >
inline typename sake::range_reference<R>::type
dispatch(R& r, T const x, sake::int_tag<0>)
{
    SAKE_ASSERT((!sake::range_traits< R const >::empty(r)));
    return sake::range_traits<R>::begin(r)[x];
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_front
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME front
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline typename sake::range_reference<R>::type
dispatch_front(R& r, sake::int_tag<2>)
{ return r.front(); }

template< class R >
inline typename sake::range_reference<R>::type
dispatch_front(R& r, sake::int_tag<1>)
{ return ::sake_range_at_private::adl_front(r); }

template< class R >
inline typename sake::range_reference<R>::type
dispatch_front(R& r, sake::int_tag<0>)
{
    SAKE_ASSERT((!sake::range_traits< R const >::empty(r)));
    return *sake::range_traits<R>::begin(r);
}

template< class R >
inline typename sake::range_reference<R>::type
dispatch(R& r, sake::begin_tag, sake::int_tag<0>)
{
    typedef typename sake::range_reference<R>::type result_type;
    typedef sake::is_convertible_wnrbt<
        boost::mpl::_1, result_type > is_convertible_wnrbt_;
    typedef typename boost_ext::mpl::
         if_< at_private::is_callable_mem_fun_front<
                  R&, result_type ( ), is_convertible_wnrbt_ >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_at_private::is_callable_front<
                  result_type ( R& ), is_convertible_wnrbt_ >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_front(r, int_tag_());
}

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_back
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME back
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class R >
inline typename sake::range_reference<R>::type
dispatch_back(R& r, sake::int_tag<2>)
{ return r.back(); }

template< class R >
inline typename sake::range_reference<R>::type
dispatch_back(R& r, sake::int_tag<1>)
{ return ::sake_range_at_private::adl_back(r); }

template< class R >
inline typename sake::range_reference<R>::type
dispatch_back(R& r, sake::int_tag<0>)
{
    SAKE_ASSERT((!sake::range_traits< R const >::empty(r)));
    return *sake::prior(sake::range_traits<R>::end(r));
}

template< class R >
inline typename sake::range_reference<R>::type
dispatch(R& r, sake::end_tag, sake::int_tag<0>)
{
    typedef typename sake::range_reference<R>::type result_type;
    typedef sake::is_convertible_wnrbt<
        boost::mpl::_1, result_type > is_convertible_wnrbt_;
    typedef typename boost_ext::mpl::
         if_< at_private::is_callable_mem_fun_back<
                  R&, result_type ( ), is_convertible_wnrbt_ >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_at_private::is_callable_back<
                  result_type ( R& ), is_convertible_wnrbt_ >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return dispatch_back(r, int_tag_());
}

} // namespace at_private

template< class R, class T >
inline typename sake::range_reference<R>::type
at(R& r, T const x)
{
    typedef typename sake::range_reference<R>::type result_type;
    typedef sake::is_convertible_wnrbt<
        boost::mpl::_1, result_type > is_convertible_wnrbt_;
    typedef typename boost_ext::mpl::
         if_< sake::has_operator_bracket<
                  R&, result_type ( T ), is_convertible_wnrbt_ >,
              sake::int_tag<3> >::type::template
    else_if < at_private::is_callable_mem_fun<
                  R&, result_type ( T ), is_convertible_wnrbt_ >,
              sake::int_tag<2> >::type::template
    else_if < ::sake_range_at_private::is_callable<
                  result_type ( R&, T ), is_convertible_wnrbt_ >,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type int_tag_;
    return at_private::dispatch(r, x, int_tag_());
}

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_AT_HPP
