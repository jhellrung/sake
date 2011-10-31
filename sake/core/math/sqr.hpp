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
 * sake::sqr(T) is implemented in terms of
 * - T::sqr(), if available; else
 * - sqr(T) (unqualified, hence subject to ADL), if available; else
 * - T::sqr_ip(), if available and the argument is an rvalue; else
 * - sqr_ip(T) (unqualified, hence subject to ADL), if available and the
 *   argument is an rvalue; else
 * - T::operator*.
 *
 * Note that for signed integral types, the return type is the corresponding
 * *unsigned* integral type.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SQR_HPP
#define SAKE_CORE_MATH_SQR_HPP

#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/unique2.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/multiply.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/private/sqr_common.hpp>
#include <sake/core/math/sqr_fwd.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace sqr_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename result_of::sqr<T>::type,
    unsigned int = dispatch_index<T>::value
>
struct dispatch;

} // namespace sqr_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::sqr<T>
 ******************************************************************************/

template< class T >
struct sqr
    : extension::sqr<T>
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

template< class T >
struct sqr_result_types
    : boost_ext::mpl::unique2<
          boost::mpl::vector2<
              T,
              typename operators::result_of::multiply<T>::type
          >
      >
{ };

template< class T >
struct sqr
    : sqr_private::dispatch< T, void >
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

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::sqr<T>::type
    operator()(T&& x) const
    { return sqr_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename result_of::sqr<
        typename boost_ext::remove_rvalue_reference< T& >::type
    >::type
    operator()(T& x) const
    {
        return sqr_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type
        >::apply(x);
    }

    template< class T >
    typename result_of::sqr< T const & >::type
    operator()(T const & x) const
    { return sqr_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
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

template< class T, class Result >
struct adl
{
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return sqr(::sake::forward<T_>(x)); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        sqr(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::sqr_result_types<
            typename ::sake::boost_ext::remove_qualifiers<T>::type
        >::type,
        type
    );
};

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
struct dispatch_index
{
    static unsigned int const _ =
        (1 << 8) * boost::is_signed<T>::value
      | (1 << 7) * boost::is_unsigned<T>::value
      | (1 << 6) * is_callable_mem_fun<T>::value
      | (1 << 5) * ::sake_sqr_private::is_callable< void ( T ) >::value
      | (1 << 4) * sqr_ip_private::is_callable_mem_fun< T&, T& ( ) >::value
      | (1 << 3) * sqr_ip_private::is_callable_mem_fun< T& >::value
      | (1 << 2) * ::sake_sqr_ip_private::is_callable< T& ( T& ) >::value
      | (1 << 1) * ::sake_sqr_ip_private::is_callable< void ( T& ) >::value
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
      | (1 << 5) * ::sake_sqr_private::is_callable< void ( T& ) >::value
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
      | (1 << 5) * ::sake_sqr_private::is_callable< void ( T const & ) >::value
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
    { return static_cast< type >(x) * static_cast< type >(x); }
};

template< class T, class Result >
struct dispatch< T, Result, 7 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    template< class T_ >
    static type apply(T_ const x)
    { return x * x; }
};

template< class T >
struct dispatch< T, void, 6 >
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        sake::declval<T>().sqr(),
        typename result_of::default_impl::sqr_result_types<
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
    { return sake::forward<T_>(x).sqr(); }
};

template< class T, class Result >
struct dispatch< T, Result, 5 >
    : ::sake_sqr_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 4 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return static_cast< type >(SAKE_AS_LVALUE( x ).sqr_ip()); }
};

template< class T, class Result >
struct dispatch< T, Result, 3 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        SAKE_AS_LVALUE( x ).sqr_ip();
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    { return ::sake_sqr_ip_private::adl< type >(SAKE_AS_LVALUE( x )); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
{
    typedef SAKE_RV_REF( T ) type;
    static type apply(type x)
    {
        ::sake_sqr_ip_private::adl< void >(SAKE_AS_LVALUE( x ));
        return static_cast< type >(x);
    }
};

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename sake::operators::result_of::multiply<
        typename boost_ext::add_reference<T>::type
    >::type type;
    template< class T_ >
    static type apply(T_& x)
    { return x * x; }
};

} // namespace sqr_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SQR_HPP
