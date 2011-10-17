/*******************************************************************************
 * sake/core/math/invert.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * invert(T const & x) -> result_of::invert<T>::type
 * struct functional::invert
 *
 * struct result_of::invert<T>
 * struct result_of::extension::invert< T, Enable = void >
 * struct result_of::default_impl::invert<T>
 *
 * Returns the (multiplicative) inverse value of its argument.
 *
 * sake::invert(T const &) is implemented in terms of
 * - T::invert(), if available; else
 * - invert(T const &) (unqualified, hence subject to ADL), if available; else
 * - returning a proxy proxy.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERT_HPP
#define SAKE_CORE_MATH_INVERT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/invert_fwd.hpp>
#include <sake/core/math/inverted.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace invert_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename result_of::invert<T>::type,
    unsigned int = dispatch_index<T>::value
>
struct dispatch;

} // namespace invert_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::invert<T>
 ******************************************************************************/

template< class T >
struct invert
    : extension::invert<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::invert< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct invert
    : default_impl::invert<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::invert<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct invert
    : invert_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          void
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * invert(T const & x) -> result_of::invert<T>::type
 * struct functional::invert
 ******************************************************************************/

namespace functional
{

struct invert
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::invert, 1 )

    template< class T >
    typename result_of::invert<T>::type
    operator()(T const & x) const
    { return invert_private::dispatch<T>::apply(x); }

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
namespace invert_adl_barrier
{ functional::invert const invert = { }; }
using namespace invert_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::invert const invert = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_invert_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME invert
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T, class Result >
struct adl
{
    static Result apply(T const & x)
    { return invert(x); }
};

template< class T >
struct adl< T, void >
{ typedef T type; };

} // namespace sake_invert_private

namespace sake
{

namespace invert_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME invert
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 2) * is_callable_mem_fun< T const & >::value
      | (1 << 1) * sake_invert_private::is_callable< void ( T const & ) >::value
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
    { return x.invert(); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : sake_invert_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef sake::inverted<T> type;
    static type apply(T const & x)
    { return type(x); }
};

} // namespace invert_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERT_HPP
