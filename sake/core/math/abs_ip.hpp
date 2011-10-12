/*******************************************************************************
 * sake/core/math/abs_ip.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * abs_ip(T& x) -> T&
 * struct functional::abs_ip
 *
 * abs_ip assigns its argument its absolute value in-place.  It automatically
 * uses ADL to find overloads of abs_ip, falling back to a default
 * implementation if ADL fails.
 *
 * The default implementation of abs_ip(T&)
 * - forwards to T::abs_ip, if available; else
 * - compares the argument to sake::zero and, if necessary, negates its argument
 *   in-place (via sake::negate_ip).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_ABS_IP_HPP
#define SAKE_CORE_MATH_ABS_IP_HPP

#include <cmath>
#include <cstdlib>

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>

#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/negate_ip.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_abs_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME abs_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) \
    typename boost_ext::add_reference<T>::type ( )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

namespace default_impl
{

template< class T >
inline T&
abs_ip(T& x);

} // namespace default_impl

} // namespace sake

namespace sake_abs_ip_private
{

template< class T >
inline T&
impl(T& x)
{
    using ::sake::default_impl::abs_ip;
    return abs_ip(x);
}

} // namespace sake_abs_ip_private

namespace sake
{

namespace result_of
{

template< class T >
struct abs_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference<T>));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * abs_ip(T& x) -> T&
 * struct functional::abs_ip
 ******************************************************************************/

namespace functional
{

struct abs_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::abs_ip, 1 )

    template< class T >
    T&
    operator()(T& x) const
    { return ::sake_abs_ip_private::impl(x); }

    int&
    operator()(int& x) const
    { return x = std::abs(x); }

    long&
    operator()(long& x) const
    { return x = std::abs(x); }

    #ifndef BOOST_NO_LONG_LONG
    long long&
    operator()(long long& x) const
    { return x = std::abs(x); }
    #endif // #ifndef BOOST_NO_LONG_LONG

    float&
    operator()(float& x) const
    { return x = std::fabs(x); }

    double&
    operator()(double& x) const
    { return x = std::fabs(x); }

    long double&
    operator()(long double& x) const
    { return x = std::fabs(x); }
};

} // namespace functional

functional::abs_ip const abs_ip = { };

namespace default_impl
{

namespace abs_ip_private
{

template<
    class T,
    bool = sake::is_callable_mem_fun_abs_ip< T& >::value,
    bool = sake::is_callable_mem_fun_abs_ip< T&, void ( ) >::value,
    bool = boost::is_signed<T>::value,
    bool = boost::is_unsigned<T>::value
>
struct dispatch;

template< class T >
struct dispatch< T, true, true, false, false >
{
    static T& apply(T& x)
    { return x.abs_ip(); }
};

template< class T >
struct dispatch< T, false, true, false, false >
{
    static T& apply(T& x)
    { x.abs_ip(); return x; }
};

template< class T >
struct dispatch< T, false, false, true, false >
{
    static T& apply(T& x)
    { return x = std::abs(x); }
};

template< class T >
struct dispatch< T, false, false, false, true >
{
    static T& apply(T& x)
    { return x; }
};

template< class T >
struct dispatch< T, false, false, false >
{
    static T& apply(T& x)
    { return x < sake::zero ? sake::negate_ip(x) : x; }
};

} // namespace abs_ip_private

template< class T >
inline T&
abs_ip(T& x)
{ return abs_ip_private::dispatch<T>::apply(x); }

inline float&
abs_ip(float& x)
{ return x = std::fabs(x); }

inline double&
abs_ip(double& x)
{ return x = std::fabs(x); }

inline long double&
abs_ip(long double& x)
{ return x = std::fabs(x); }

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_ABS_IP_HPP
