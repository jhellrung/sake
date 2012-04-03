/*******************************************************************************
 * sake/core/math/abs.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * abs(T&& x) -> result_of::abs<T>::type
 * struct functional::abs
 *
 * struct result_of::abs<T>
 * struct result_of::extension::abs< T, Enable = void >
 * struct result_of::default_impl::abs<T>
 *
 * Returns the absolute value of its argument.
 *
 * sake::abs(T) is implemented in terms of
 * - std::[f]abs if T is a builtin floating point or integral type; else
 * - T::abs(), if available; else
 * - abs(T) (unqualified, hence subject to ADL), if available; else
 * - T::abs_ip(), if available and the argument is an rvalue; else
 * - abs_ip(T) (unqualified, hence subject to ADL), if available and the
 *   argument is an rvalue; else
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

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>

#include <sake/boost_ext/mpl/unique2.hpp>
#include <sake/boost_ext/type_traits/common_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/unary_minus.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/abs_fwd.hpp>
#include <sake/core/math/private/abs_common.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

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
    : extension::abs<T>
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
          boost::mpl::vector2<
              T,
              typename boost_ext::common_type<
                  typename operators::result_of::unary_minus<T>::type,
                  T
              >::type
          >
      >
{ };

template< class T >
struct abs
    : abs_private::dispatch< T, void >
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

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::abs<T>::type
    operator()(T&& x) const
    { return abs_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::abs<
        typename boost_ext::remove_rvalue_reference< T& >::type
    >::type
    operator()(T& x) const
    {
        return abs_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type
        >::apply(x);
    }

    template< class T >
    typename result_of::abs< T const & >::type
    operator()(T const & x) const
    { return abs_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

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

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace abs_adl_barrier
{ functional::abs const abs = { }; }
using namespace abs_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::abs const abs = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

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
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return abs(::sake::forward<T_>(x)); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename abs(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::abs_result_types<
            typename ::sake::boost_ext::remove_qualifiers<T>::type
        >::type,
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
        (1 << 8) * boost::is_signed<T>::value
      | (1 << 7) * boost::is_unsigned<T>::value
      | (1 << 6) * is_callable_mem_fun<T>::value
      | (1 << 5) * ::sake_abs_private::is_callable< void ( T ) >::value
      | (1 << 4) * abs_ip_private::is_callable_mem_fun< T&, T& ( ) >::value
      | (1 << 3) * abs_ip_private::is_callable_mem_fun< T& >::value
      | (1 << 2) * ::sake_abs_ip_private::is_callable< T& ( T& ) >::value
      | (1 << 1) * ::sake_abs_ip_private::is_callable< void ( T& ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct dispatch_index< T& >
{
    static unsigned int const _ =
        (1 << 8) * boost::is_signed<T>::value
      | (1 << 7) * boost::is_unsigned<T>::value
      | (1 << 6) * is_callable_mem_fun< T& >::value
      | (1 << 5) * ::sake_abs_private::is_callable< void ( T& ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct dispatch_index< T const & >
{
    static unsigned int const _ =
        (1 << 8) * boost::is_signed<T>::value
      | (1 << 7) * boost::is_unsigned<T>::value
      | (1 << 6) * is_callable_mem_fun< T const & >::value
      | (1 << 5) * ::sake_abs_private::is_callable< void ( T const & ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T, class Result >
struct dispatch< T, Result, 8 >
{
    typedef typename boost::make_unsigned<
        typename boost_ext::remove_qualifiers<T>::type
    >::type type;
    template< class T_ >
    static type apply(T_ const x)
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
struct dispatch< T, Result, 7 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    template< class T_ >
    static type apply(T_ const x)
    { return x; }
};

template< class T >
struct dispatch< T, void, 6 >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval<T>().abs(),
        typename result_of::default_impl::abs_result_types<
            typename boost_ext::remove_qualifiers<T>::type
        >::type,
        type
    );
};

template< class T, class Result >
struct dispatch< T, Result, 6 >
{
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return sake::forward<T_>(x).abs(); }
};

template< class T, class Result >
struct dispatch< T, Result, 5 >
    : ::sake_abs_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 4 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return static_cast< type >(SAKE_AS_LVALUE( x ).abs_ip()); }
};

template< class T, class Result >
struct dispatch< T, Result, 3 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        SAKE_AS_LVALUE( x ).abs_ip();
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return ::sake_abs_ip_private::adl< type >(SAKE_AS_LVALUE( x )); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        ::sake_abs_ip_private::adl< void >(SAKE_AS_LVALUE( x ));
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename boost_ext::remove_qualifiers<
        typename boost_ext::common_type<
            typename sake::operators::result_of::unary_minus<T>::type, T
        >::type
    >::type type;
    template< class T_ >
    static type apply(SAKE_FWD2_REF( T_ ) x)
    {
        return SAKE_AS_LVALUE( x ) < sake::zero ?
               -sake::forward<T_>(x) : sake::forward<T_>(x);
    }
};

} // namespace abs_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_HPP
