/*******************************************************************************
 * sake/core/math/abs.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * abs(T const & x) -> result_of::abs<T>::type
 * struct functional::abs
 *
 * struct result_of::abs<T>
 * struct result_of::extension::abs< T, Enable = void >
 * struct result_of::default_impl::abs<T>
 *
 * abs returns the absolute value of its argument.  It automatically uses ADL to
 * find overloads of abs, falling back to a default implementation if ADL fails.
 *
 * The default implementation of abs(T const &)
 * - forwards to T::abs, if available; else
 * - compares the argument to sake::zero and either returns the argument or its
 *   negation.
 *
 * Note that for signed integral types, the return type is the corresponding
 * *unsigned* integral type.  This behavior differs, specifically, from
 * std::abs.  This choice of return type allows the absolute value of the
 * minimum value of a given integral type to be accurately represented.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_HPP
#define SAKE_CORE_MATH_ABS_HPP

#include <cmath>

#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/functional/operators/unary_minus.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/abs_fwd.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace abs_private
{

template< class T >
struct impl;

} // namespace abs_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::abs<T>
 ******************************************************************************/

template< class T >
struct abs
    : extension::abs<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

/*******************************************************************************
 * struct result_of::extension::abs< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct abs
    : default_impl::abs<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::abs<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct abs
    : abs_private::impl<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * abs(T const & x) -> result_of::abs<T>::type
 * struct functional::abs
 ******************************************************************************/

namespace functional
{

struct abs
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::abs, 1 )

    template< class T >
    typename result_of::abs<T>::type
    operator()(T const & x) const
    { sake::abs_private::impl<T>::apply(x); }

    float
    operator()(float const x) const
    { return std::fabs(x); }
    double
    operator()(double const x) const
    { return std::fabs(x); }
    long double
    operator()(long double const x) const
    { return std::fabs(x); }
};

} // namespace functional

functional::abs const abs = { };

} // namespace sake

namespace sake_abs_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME abs
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T >
inline typename ::sake::result_of::abs<T>::type
adl(T const & x)
{ return abs(x); }

} // namespace sake_abs_private

namespace sake
{

namespace abs_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME abs
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) \
    typename boost_ext::remove_qualifiers<T>::type ( )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template<
    class T,
    bool = ::sake_abs_private::is_callable< T ( T const & ) >::value
>
struct dispatch_on_is_callable;

template<
    class T,
    bool = is_callable_mem_fun< T const &, T ( ) >::value
>
struct dispatch_on_is_callable_mem_fun;

template<
    class T,
    bool = boost::is_signed<T>::value,
    bool = boost::is_unsigned<T>::value
>
struct dispatch_on_is_signed_unsigned;

template< class T >
struct impl
    : dispatch_on_is_callable<T>
{ };

template< class T >
struct dispatch_on_is_callable< T, true >
{
    typedef T type;
    static type apply(T const & x)
    { return ::sake_abs_private::adl(x); }
};

template< class T >
struct dispatch_on_is_callable< T, false >
    : dispatch_on_is_callable_mem_fun<T>
{ };

template< class T >
struct dispatch_on_is_callable_mem_fun< T, true >
{
    typedef T type;
    static type apply(T const & x)
    { return x.abs(); }
};

template< class T >
struct dispatch_on_is_callable_mem_fun< T, false >
    : dispatch_on_is_signed_unsigned<T>
{ };

template< class T >
struct dispatch_on_is_signed_unsigned< T, true, false >
{
    typedef typename boost::make_unsigned<T>::type type;
    static type apply(T const x)
    {
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#endif // #ifdef _MSC_VER
        return x < 0 ? -static_cast< type >(x) : static_cast< type >(x);
#ifdef _MSC_VER
#pragma warning( pop )
#endif // #ifdef _MSC_VER
    }
};

template< class T >
struct dispatch_on_is_signed_unsigned< T, false, true >
{
    typedef T type;
    static type apply(T const x)
    { return x; }
};

template< class T >
struct dispatch_on_is_signed_unsigned< T, false, false >
{
    typedef typename boost_ext::common_type<
        typename operators::result_of::unary_minus<T>::type, T >::type type;
    static type apply(T const & x)
    { return x < sake::zero ? -x : x; }
};

} // namespace abs_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_HPP
