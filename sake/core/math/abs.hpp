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
 * Returns the absolute value of its argument.
 *
 * sake::abs(T const &) is implemented in terms of
 * - std::[f]abs if T is a builtin floating point or integral type; else
 * - T::abs(), if available; else
 * - abs(T const &) (unqualified, hence subject to ADL), if available; else
 * - comparison to sake::zero, returning either the argument or its negation.
 *
 * Note that for signed integral types, the return type is the corresponding
 * *unsigned* integral type.  This behavior differs, specifically, from
 * std::abs.  This choice of return type allows the absolute value of the
 * minimum value of a given integral type to be accurately represented.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_HPP
#define SAKE_CORE_MATH_ABS_HPP

#include <cmath>

#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/unique2.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/unary_minus.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/abs_fwd.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace abs_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename result_of::abs<T>::type,
    unsigned int = dispatch_index<T>::value
>
struct dispatch;

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
struct abs_result_types
    : boost_ext::mpl::unique2<
          boost::mpl::vector3<
              T,
              T const &,
              typename boost_ext::common_type<
                  typename operators::result_of::unary_minus<T>::type,
                  T
              >::type
          >
      >
{ };

template< class T >
struct abs
    : sake::abs_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          void
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
    { return abs_private::dispatch<T>::apply(x); }

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

template< class T, class Result >
struct adl
{
    static Result apply(T const & x)
    { return abs(x); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        abs(::sake::declcref<T>()),
        typename ::sake::result_of::default_impl::abs_result_types<T>::type,
        type
    );
};

} // namespace sake_abs_private

namespace sake
{

namespace abs_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME abs
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 4) * boost::is_signed<T>::value
      | (1 << 3) * boost::is_unsigned<T>::value
      | (1 << 2) * is_callable_mem_fun< T const & >::value
      | (1 << 1) * sake_abs_private::is_callable< void ( T const & ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T, class Result >
struct dispatch< T, Result, 4 >
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

template< class T, class Result >
struct dispatch< T, Result, 3 >
{
    typedef T type;
    static type apply(T const x)
    { return x; }
};

template< class T >
struct dispatch< T, void, 2 >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        sake::declcref<T>().abs(),
        typename result_of::default_impl::abs_result_types<T>::type,
        type
    );
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    static Result apply(T const & x)
    { return x.abs(); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : ::sake_abs_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename boost_ext::common_type<
        typename sake::operators::result_of::unary_minus<T>::type,
        T
    >::type type;
    static type apply(T const & x)
    { return x < sake::zero ? -x : x; }
};

} // namespace abs_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_HPP
