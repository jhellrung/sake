/*******************************************************************************
 * sake/core/math/negate_ip.hpp
 *
 * Copyright 2010, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * negate_ip(T& x) -> T&
 * struct functional::negate_ip
 *
 * negate_ip negates its argument in-place.  It automatically uses ADL to find
 * overloads of negate_ip, falling back to a default implementation if ADL
 * fails.
 *
 * The default implementation of negate_ip(T&)
 * - forwards to T::negate_ip, if available; else
 * - assigns the argument to its negated value (via operator-).
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_NEGATE_IP_HPP
#define SAKE_CORE_MATH_NEGATE_IP_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>

#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_negate_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME negate_ip
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) \
    typename boost_ext::add_reference<T>::type ( )
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

namespace default_impl
{

template< class T >
inline T&
negate_ip(T& x);

} // namespace default_impl

} // namespace sake

namespace sake_negate_ip_private
{

template< class T >
inline T&
impl(T& x)
{
    using ::sake::default_impl::negate_ip;
    return negate_ip(x);
}

} // namespace sake_negate_ip_private

namespace sake
{

namespace result_of
{

template< class T >
struct negate_ip
{
    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference<T>::value));
    typedef T type;
};

} // namespace result_of

/*******************************************************************************
 * negate_ip(T& x) -> T&
 * struct functional::negate_ip
 ******************************************************************************/

namespace functional
{

struct negate_ip
{
    SAKE_RESULT_FROM_METAFUNCTION( result_of::negate_ip, 1 )

    template< class T >
    T& operator()(T& x) const
    { return ::sake_negate_ip_private::impl(x); }
};

} // namespace functional

functional::negate_ip const negate_ip = { };

namespace default_impl
{

namespace negate_ip_private
{

template<
    class T,
    bool = sake::is_callable_mem_fun_negate_ip< T& >::value,
    bool = sake::is_callable_mem_fun_negate_ip< T&, void ( ) >::value
>
struct dispatch;

template< class T >
struct dispatch< T, true, true >
{
    static T& apply(T& x)
    { return x.negate_ip(); }
};

template< class T >
struct dispatch< T, false, true >
{
    static T& apply(T& x)
    { x.negate_ip(); return x; }
};

template< class T >
struct dispatch< T, false, false >
{
    static T& apply(T& x)
    { return x = -x; }
};

} // namespace negate_ip_private

template< class T >
inline T&
negate_ip(T& x)
{ return negate_ip_private::dispatch<T>::apply(x); }

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_NEGATE_IP_HPP
