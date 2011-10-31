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
 * sake::inv(T) is implemented in terms of
 * - T::inv(), if available; else
 * - inv(T) (unqualified, hence subject to ADL), if available; else
 * - T::inv_ip(), if available and the argument is an rvalue; else
 * - inv_ip(T) (unqualified, hence subject to ADL), if available and the
 *   argument is an rvalue; else
 * - returning a proxy.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERT_HPP
#define SAKE_CORE_MATH_INVERT_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/unique2.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/divide.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/inv_fwd.hpp>
#include <sake/core/math/inverse.hpp>
#include <sake/core/math/private/inv_common.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/declval.hpp>
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
    : extension::inv<T>
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
struct inv_result_types
    : boost_ext::mpl::unique2<
          boost::mpl::vector2<
              T,
              typename operators::result_of::divide<T>::type
          >
      >
{ };

template< class T >
struct inv
    : inv_private::dispatch< T, void >
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

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::inv<T>::type
    operator()(T&& x) const
    { return inv_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::inv<
        typename boost_ext::remove_rvalue_reference< T& >::type
    >::type
    operator()(T& x) const
    {
        return inv_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type
        >::apply(x);
    }

    template< class T >
    typename result_of::inv< T const & >::type
    operator()(T const & x) const
    { return inv_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

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
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return inv(::sake::forward<T_>(x)); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        inv(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::inv_result_types<
            typename ::sake::boost_ext::remove_qualifiers<T>::type
        >::type,
        type
    );
};

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
        (1 << 7) * boost::is_floating_point<T>::value
      | (1 << 6) * is_callable_mem_fun<T>::value
      | (1 << 5) * ::sake_inv_private::is_callable< void ( T ) >::value
      | (1 << 4) * inv_ip_private::is_callable_mem_fun< T&, T& ( ) >::value
      | (1 << 3) * inv_ip_private::is_callable_mem_fun< T& >::value
      | (1 << 2) * ::sake_inv_ip_private::is_callable< T& ( T& ) >::value
      | (1 << 1) * ::sake_inv_ip_private::is_callable< void ( T& ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct dispatch_index< T& >
{
    static unsigned int const _ =
        (1 << 7) * boost::is_floating_point< typename boost::remove_const<T>::type >::value
      | (1 << 6) * is_callable_mem_fun< T& >::value
      | (1 << 5) * ::sake_inv_private::is_callable< void ( T& ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T, class Result >
struct dispatch< T, Result, 7 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    static type apply(type const x)
    { return 1/x; }
};

template< class T >
struct dispatch< T, void, 6 >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        sake::declval<T>().inv(),
        typename result_of::default_impl::inv_result_types<
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
    { return sake::forward<T_>(x).inv(); }
};

template< class T, class Result >
struct dispatch< T, Result, 5 >
    : ::sake_inv_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 4 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return static_cast< type >(SAKE_AS_LVALUE( x ).inv_ip()); }
};

template< class T, class Result >
struct dispatch< T, Result, 3 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        SAKE_AS_LVALUE( x ).inv_ip();
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return ::sake_inv_ip_private::adl< type >(SAKE_AS_LVALUE( x )); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        ::sake_inv_ip_private::adl< void >(SAKE_AS_LVALUE( x ));
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef sake::inverse<
        typename boost_ext::remove_qualifiers<T>::type
    > type;
    template< class T_ >
    static type apply(SAKE_FWD2_REF( T_ ) x)
    { return type(sake::forward<T_>(x)); }
};

} // namespace inv_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERT_HPP
