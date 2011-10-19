/*******************************************************************************
 * sake/core/math/sqr.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sqr(T const & x) -> result_of::sqr<T>::type
 * struct functional::sqr
 *
 * struct result_of::sqr<T>
 * struct result_of::extension::sqr< T, Enable = void >
 * struct result_of::default_impl::sqr<T>
 *
 * Returns the squared value of its argument.
 *
 * sake::sqr(T const &) is implemented in terms of
 * - T::sqr(), if available; else
 * - sqr(T const &) (unqualified, hence subject to ADL), if available; else
 * - T::operator*.
 *
 * Note that for signed integral types, the return type is the corresponding
 * *unsigned* integral type.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_HPP
#define SAKE_CORE_MATH_ABS_HPP

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/sqr_fwd.hpp>
#include <sake/core/utility/dispatch_priority_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace sqr_private
{

template< class T >
inline typename result_of::sqr<T>::type
impl(T const &);

} // namespace sqr_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::sqr<T>
 ******************************************************************************/

template< class T >
struct sqr
    : extension::sqr<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::sqr< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct sqr
    : default_impl::sqr<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::sqr<T>
 ******************************************************************************/

namespace default_impl
{

namespace sqr_private
{

template< class T, bool = boost::is_signed<T>::value >
struct dispatch;

template< class T >
struct disable< T, false >
    : sake::operators::result_of::multiplies<T>
{ };

template< class T >
struct disable< T, true >
    : boost::make_unsigned<T>
{ };

} // namespace sqr_private

template< class T >
struct sqr
    : sqr_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * sqr(T const & x) -> result_of::sqr<T>::type
 * struct functional::sqr
 ******************************************************************************/

namespace functional
{

struct sqr
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::sqr, 1 )

    template< class T >
    typename result_of::sqr<T>::type
    operator()(T const & x) const
    { return sqr_private::impl(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sqr_adl_barrier
{ functional::sqr const sqr = { }; }
using namespace sqr_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::sqr const sqr = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_sqr_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME sqr
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T >
typename ::sake::result_of::sqr<T>::type
adl(T const & x)
{ return sqr(x); }

} // namespace sake_sqr_private

namespace sake
{

namespace sqr_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME sqr
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
inline typename boost::enable_if_c<
    boost::is_signed<T>::value,
    typename result_of::sqr<T>::type
>::type
dispatch(T const & x, sake::dispatch_priority_tag<3>)
{ return static_cast< typename result_of::sqr<T>::type >(x*x); }

template< class T >
inline typename boost::enable_if_c<
    is_callable_mem_fun< T const & >::value,
    typename result_of::sqr<T>::type
>::type
dispatch(T const & x, sake::dispatch_priority_tag<2>)
{ return x.sqr(); }

template< class T >
inline typename boost::enable_if_c<
    ::sake_sqr_private::is_callable< void ( T const & ) >::value,
    typename result_of::sqr<T>::type
>::type
dispatch(T const & x, sake::dispatch_priority_tag<1>)
{ return ::sake_sqr_private::adl(x); }

template< class T >
inline typename result_of::sqr<T>::type
dispatch(T const & x, sake::dispatch_priority_tag<0>)
{ return x*x; }

template< class T >
inline typename result_of::sqr<T>::type
impl(T const & x)
{ return sqr_private::disable(x, sake::dispatch_priority_tag<3>()); }

} // namespace sqr_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_HPP
