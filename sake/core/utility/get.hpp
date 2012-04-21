/*******************************************************************************
 * sake/core/utility/get.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * get(T&& x) -> result_of::get<T>::type
 * struct functional::get
 *
 * struct result_of::get<T>
 * struct result_of::extension::get< T, Enable = void >
 * struct result_of::default_impl::get<T>
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_GET_HPP
#define SAKE_CORE_UTILITY_GET_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/get_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace result_of
{

/*******************************************************************************
 * struct result_of::get<T>
 ******************************************************************************/

template< class T >
struct get
    : sake::result_of::extension::get<
          typename boost_ext::remove_rvalue_reference<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::get< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct get
    : sake::result_of::default_impl::get<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::get<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct get
    : boost_ext::add_rvalue_reference<T>
{ };

} // namespace default_impl

} // namespace result_of

namespace get_private
{

template< class T >
inline typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type
impl(SAKE_FWD2_REF( T ) x);

} // namespace get_private

/*******************************************************************************
 * get(T&& x) -> result_of::get<T>::type
 * struct functional::get
 ******************************************************************************/

namespace functional
{

struct get
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::get, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::get<T>::type
    operator()(T&& x) const
    { return get_private::impl(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::get< T& >::type
    operator()(T& x) const
    { return get_private::impl(x); }

    template< class T >
    typename sake::result_of::get< T const & >::type
    operator()(T const & x) const
    { return get_private::impl(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace get_adl_barrier
{ sake::functional::get const get = { }; }
using namespace get_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::get const get = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_get_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME get
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class Result, class T >
inline Result
adl(SAKE_FWD2_REF( T ) x)
{ return get(sake::forward<T>(x)); }

} // namespace sake_get_private

namespace sake
{

namespace get_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME get
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
inline typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type
dispatch(SAKE_FWD2_REF( T ) x, sake::int_tag<2>)
{ return sake::forward<T>(x).get(); }

template< class T >
inline typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type
dispatch(SAKE_FWD2_REF( T ) x, sake::int_tag<1>)
{
    typedef typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type result_type;
    return ::sake_get_private::adl< result_type >(sake::forward<T>(x));
}

template< class T >
inline typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type
dispatch(SAKE_FWD2_REF( T ) x, sake::int_tag<0>)
{ return sake::forward<T>(x); }

template< class T >
inline typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type
impl(SAKE_FWD2_REF( T ) x)
{
    typedef typename sake::result_of::get< SAKE_FWD2_PARAM( T ) >::type result_type;
    typedef typename boost_ext::mpl::
         if_<
        get_private::is_callable_mem_fun< SAKE_FWD2_PARAM( T ), result_type ( ) >,
        sake::int_tag<2>
    >::type::template
    else_if <
        ::sake_get_private::is_callable< result_type ( SAKE_FWD2_PARAM( T ) ) >,
        sake::int_tag<1>
    >::type::template
    else_   <
        sake::int_tag<0>
    >::type int_tag_;
    return get_private::dispatch(sake::forward<T>(x), int_tag_());
}

} // namespace get_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_GET_HPP
