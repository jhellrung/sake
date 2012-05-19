/*******************************************************************************
 * sake/core/math/sqr.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sqr(T& x) -> result_of::sqr<T>::type
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

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/config.hpp>
#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/functional/operators/multiply.hpp>
#include <sake/core/introspection/has_operator_multiply.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/private/sqr_common.hpp>
#include <sake/core/math/sqr_fwd.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace sqr_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename sake::result_of::sqr<T>::type,
    int = dispatch_index<T>::value
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
{
    typedef typename sake::result_of::extension::sqr<
        typename boost_ext::remove_rvalue_reference<T>::type
    >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
};

/*******************************************************************************
 * struct result_of::extension::sqr< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct sqr
    : sake::result_of::default_impl::sqr<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::sqr<T>
 ******************************************************************************/

namespace default_impl
{

namespace sqr_private
{

template< class T, bool = sake::has_operator_multiply<T>::value >
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
        typename boost_ext::remove_qualifiers<
            typename sake::operators::result_of::multiply<T>::type
        >::type
    > type;
};

} // namespace sqr_private

template< class T >
struct sqr_result_types
    : sake::result_of::default_impl::sqr_private::result_types_dispatch<T>
{ };

template< class T >
struct sqr
    : sake::sqr_private::dispatch< T, void >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * sqr(T&& x) -> result_of::sqr<T>::type
 * struct functional::sqr
 ******************************************************************************/

namespace functional
{

struct sqr
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::sqr, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::sqr<T>::type
    operator()(T&& x) const
    { return sake::sqr_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::sqr< T& >::type
    operator()(T& x) const
    {
        return sake::sqr_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type
        >::apply(x);
    }

    template< class T >
    typename sake::result_of::sqr< T const & >::type
    operator()(T const & x) const
    { return sake::sqr_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sqr_adl_barrier
{ sake::functional::sqr const sqr = { }; }
using namespace sqr_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::sqr const sqr = { };
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
    typedef typename ::sake::boost_ext::add_rvalue_reference<T>::type fwd_type;
    static Result apply(fwd_type x)
    { return sqr(static_cast< fwd_type >(x)); }
};

template< class T >
struct adl_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sqr(::sake::declval<T>()),
        typename ::sake::result_of::default_impl::sqr_result_types<T>::type,
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
        ::sake::result_of::sqr< typename boost::remove_cv<T>::type >,
        ::boost::mpl::identity< maybe_type >
    >::type type;
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
private:
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename boost_ext::add_reference<T>::type ref_type;
public:
    static int const value = boost_ext::mpl::
         if_< boost::is_signed< noqual_type >              , sake::int_tag<8> >::type::template
    else_if < boost::is_unsigned< noqual_type >            , sake::int_tag<7> >::type::template
    else_if < is_callable_mem_fun<T>                       , sake::int_tag<6> >::type::template
    else_if < ::sake_sqr_private::is_callable< void ( T ) >, sake::int_tag<5> >::type::template
#ifndef BOOST_NO_RVALUE_REFERENCES
    else_if < boost_ext::is_reference<T>, sake::int_tag<0> >::type::template
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    else_if_not< sake::has_move_emulation<T>, sake::int_tag<0> >::type::template
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
    else_if < sqr_ip_private::is_callable_mem_fun< ref_type, ref_type ( ) >, sake::int_tag<4> >::type::template
    else_if < sqr_ip_private::is_callable_mem_fun< ref_type >              , sake::int_tag<3> >::type::template
    else_if < ::sake_sqr_ip_private::is_callable< ref_type ( ref_type ) >  , sake::int_tag<2> >::type::template
    else_if < ::sake_sqr_ip_private::is_callable< void ( ref_type ) >      , sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
};

template< class T, class Result >
struct dispatch< T, Result, 8 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type noqual_type;
    typedef typename boost::make_unsigned< noqual_type >::type type;
    static type apply(noqual_type const x)
    { return static_cast< type >(x * x); }
};

template< class T, class Result >
struct dispatch< T, Result, 7 >
{
    typedef typename boost_ext::remove_qualifiers<T>::type type;
    static type apply(type const x)
    { return x * x; }
};

template< class T >
struct dispatch6_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval<T>().sqr(),
        typename sake::result_of::default_impl::sqr_result_types<T>::type,
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
        result_of::sqr< typename boost::remove_cv<T>::type >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

template< class T, class Result >
struct dispatch< T, Result, 6 >
{
    typedef typename boost_ext::add_rvalue_reference<T>::type fwd_type;
    static Result apply(fwd_type x)
    { return static_cast< fwd_type >(x).sqr(); }
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
    typedef typename boost_ext::add_rvalue_reference<T>::type fwd_type;
    typedef typename sake::operators::result_of::multiply<
        typename boost_ext::add_reference<T>::type
    >::type type;
    static type apply(fwd_type x)
    { return SAKE_AS_LVALUE( x ) * SAKE_AS_LVALUE( x ); }
};

} // namespace sqr_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SQR_HPP
