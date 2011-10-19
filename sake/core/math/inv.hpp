/*******************************************************************************
 * sake/core/math/inv.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * inv(T const & x) -> result_of::inv<T>::type
 * struct functional::inv
 *
 * struct result_of::inv<T>
 * struct result_of::extension::inv< T, Enable = void >
 * struct result_of::default_impl::inv<T>
 *
 * Returns the (multiplicative) inverse value of its argument.
 *
 * sake::inv(T const &) is implemented in terms of
 * - T::inv(), if available; else
 * - inv(T const &) (unqualified, hence subject to ADL), if available; else
 * - returning a proxy proxy.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERT_HPP
#define SAKE_CORE_MATH_INVERT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/inv_fwd.hpp>
#include <sake/core/math/inverse.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace inv_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename result_of::inv<T>::type,
    unsigned int = dispatch_index<T>::value
>
struct dispatch;

} // namespace inv_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::inv<T>
 ******************************************************************************/

template< class T >
struct inv
    : extension::inv<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::inv< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct inv
    : default_impl::inv<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::inv<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct inv
    : inv_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          void
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * inv(T const & x) -> result_of::inv<T>::type
 * struct functional::inv
 ******************************************************************************/

namespace functional
{

struct inv
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::inv, 1 )

    template< class T >
    typename result_of::inv<T>::type
    operator()(T const & x) const
    { return inv_private::dispatch<T>::apply(x); }

    float
    operator()(float const x) const
    { return 1/x; }
    double
    operator()(double const x) const
    { return 1/x; }
    long double
    operator()(long double const x) const
    { return 1/x; }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace inv_adl_barrier
{ functional::inv const inv = { }; }
using namespace inv_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::inv const inv = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_inv_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME inv
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T, class Result >
struct adl
{
    static Result apply(T const & x)
    { return inv(x); }
};

template< class T >
struct adl< T, void >
{ typedef T type; };

} // namespace sake_inv_private

namespace sake
{

namespace inv_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME inv
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 2) * is_callable_mem_fun< T const & >::value
      | (1 << 1) * sake_inv_private::is_callable< void ( T const & ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct dispatch< T, void, 2 >
{ typedef T type; };

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    static Result apply(T const & x)
    { return x.inv(); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : sake_inv_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef sake::inverse<T> type;
    static type apply(T const & x)
    { return type(x); }
};

} // namespace inv_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERT_HPP
