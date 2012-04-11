/*******************************************************************************
 * sake/core/math/inv.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * inv(T&& x) -> result_of::inv<T>::type
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

#ifndef SAKE_CORE_MATH_INV_HPP
#define SAKE_CORE_MATH_INV_HPP

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/uint.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/divide.hpp>
#include <sake/core/introspection/has_operator_divide.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/inv_fwd.hpp>
#include <sake/core/math/inverse.hpp>
#include <sake/core/math/private/inv_common.hpp>
#include <sake/core/math/static_intlog2.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
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
{
    typedef typename extension::inv<
        typename boost_ext::remove_rvalue_reference<T>::type
    >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
};

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

namespace inv_private
{

template< class T, bool = sake::has_operator_divide<T>::value >
struct result_types_dispatch;

template< class T >
struct result_types_dispatch< T, false >
{
    typedef boost::mpl::vector1<
        typename boost_ext::remove_qualifiers<T>::type
    > type;
};

template< class T >
struct result_types_dispatch< T, true >
{
    typedef boost::mpl::vector2<
        typename boost_ext::remove_qualifiers<T>::type,
        typename boost_ext::remove_rvalue_reference<
            typename operators::result_of::divide<T>::type
        >::type
    > type;
};

} // namespace inv_private

template< class T >
struct inv_result_types
    : inv_private::result_types_dispatch<T>
{ };

template< class T >
struct inv
    : sake::inv_private::dispatch< T, void >
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
    typename result_of::inv< T& >::type
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
struct adl_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename inv(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::inv_result_types<T>::type,
        type
    );
};

template< class T >
struct adl< T, void >
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_cv_or<T>::value));
    typedef typename adl_impl<T>::type type;
};

template< class T >
struct adl< T&, void >
{
private:
    typedef typename adl_impl< T& >::type maybe_type;
public:
    typedef typename ::boost::mpl::eval_if_c<
        ::boost::is_void< maybe_type >::value,
        ::sake::result_of::inv< typename boost::remove_cv<T>::type >,
        ::boost::mpl::identity< maybe_type >
    >::type type;
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

using boost_ext::mpl::uint;

template< class T >
struct dispatch_index
{
private:
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename boost_ext::add_reference<T>::type ref_type;
public:
    static unsigned int const value = boost_ext::mpl::
             if_< boost::is_floating_point< noqual_type >      , uint<7> >::type::template
        else_if < is_callable_mem_fun<T>                       , uint<6> >::type::template
        else_if < ::sake_inv_private::is_callable< void ( T ) >, uint<5> >::type::template
#ifndef BOOST_NO_RVALUE_REFERENCES
        else_if < boost_ext::is_reference<T>, uint<0> >::type::template
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
        else_if_not< sake::is_movable<T>, uint<0> >::type::template
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
        else_if < inv_ip_private::is_callable_mem_fun< ref_type, ref_type ( ) >, uint<4> >::type::template
        else_if < inv_ip_private::is_callable_mem_fun< ref_type >              , uint<3> >::type::template
        else_if < ::sake_inv_ip_private::is_callable< ref_type ( ref_type ) >  , uint<2> >::type::template
        else_if < ::sake_inv_ip_private::is_callable< void ( ref_type ) >      , uint<1> >::type::template
        else_   < uint<0> >::type::value;
};

template< class T, class Result >
struct dispatch< T, Result, 7 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    static type apply(type const x)
    { return 1/x; }
};

template< class T >
struct dispatch6_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval<T>().inv(),
        typename result_of::default_impl::inv_result_types<T>::type,
        type
    );
};

template< class T >
struct dispatch< T, void, 6 >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    typedef typename dispatch6_impl<T>::type type;
};

template< class T >
struct dispatch< T&, void, 6 >
{
private:
    typedef typename dispatch6_impl< T& >::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        result_of::inv< typename boost::remove_cv<T>::type >,
        boost::mpl::identity< maybe_type >
    >::type type;
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

#endif // #ifndef SAKE_CORE_MATH_INV_HPP
