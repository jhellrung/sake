/*******************************************************************************
 * sake/core/math/sign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sign(T&& x) -> result_of::sign<T>::type
 * struct functional::sign
 *
 * struct result_of::sign<T>
 * struct result_of::extension::sign< T, Enable = void >
 * struct result_of::default_impl::sign<T>
 *
 * Teturns the sign of its argument (either a sake::sign_t or, in the event
 * that the sign could be indeterminate, a sake::fuzzy_sign_t).
 *
 * sake::sign(T) is implemented in terms of
 * - T::sign(), if available; else
 * - sign(T) (unqualified, hence subject to ADL), if available; else
 * - comparison with sake::zero (via sake::cmp).
 *
 * The default implementation of the result_of::sign metafunction is evaluated
 * based on the result of the first available implementation above and is able
 * to detect result types convertible to int, sake::sign_t, and
 * sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIGN_HPP
#define SAKE_CORE_MATH_SIGN_HPP

#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/private/sign_common.hpp>
#include <sake/core/math/sign_fwd.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/workaround.hpp>

namespace sake
{

namespace sign_private
{

template< class T >
struct dispatch_index;

template<
    class T,
    class Result = typename sake::result_of::sign<T>::type,
    int = dispatch_index<T>::value
>
struct dispatch;

} // namespace sign_private

namespace result_of
{

/*******************************************************************************
 * struct result_of::sign<T>
 ******************************************************************************/

template< class T >
struct sign
{
    typedef typename extension::sign<
        typename boost_ext::remove_rvalue_reference<T>::type
    >::type type;
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
};

/*******************************************************************************
 * struct result_of::extension::sign< T, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Enable /*= void*/ >
struct sign
    : default_impl::sign<T>
{ };

} // namespace extension

/*******************************************************************************
 * struct result_of::default_impl::sign<T>
 ******************************************************************************/

namespace default_impl
{

template< class T >
struct sign
    : sign_private::dispatch< T, void >
{ };

} // namespace default_impl

} // namespace result_of

/*******************************************************************************
 * sign(T const & x) -> result_of::sign<T>::type
 * struct functional::sign
 ******************************************************************************/

namespace functional
{

struct sign
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::sign, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::sign<T>::type
    operator()(T&& x) const
    { return sign_private::dispatch<T>::apply(sake::forward<T>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename sake::result_of::sign< T& >::type
    operator()(T& x) const
    {
        return sign_private::dispatch<
            typename boost_ext::remove_rvalue_reference< T& >::type
        >::apply(x);
    }

    template< class T >
    typename sake::result_of::sign< T const & >::type
    operator()(T const & x) const
    { return sign_private::dispatch< T const & >::apply(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sign_adl_barrier
{ sake::functional::sign const sign = { }; }
using namespace sign_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::functional::sign const sign = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace sake

namespace sake_sign_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME    is_callable
#define SAKE_INTROSPECTION_FUNCTION_NAME sign
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS ( 1, 1 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

template< class T, class Result >
struct adl
{
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return static_cast< Result >(sign(::sake::forward< T_ >(x))); }
};

template< class T >
class adl_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sign(::sake::declval<T>()),
        ::sake::result_of::default_impl::sign_result_types,
        nominal_type
    );
public:
    typedef typename ::boost::mpl::if_c<
        ::boost::is_same< nominal_type, int >::value,
        ::sake::sign_t,
        nominal_type
    >::type type;
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
        ::sake::result_of::sign< typename ::boost::remove_cv<T>::type >,
        ::boost::mpl::identity< maybe_type >
    >::type type;
};

} // namespace sake_sign_private

namespace sake
{

namespace sign_private
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME sign
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

template< class T >
struct dispatch_index
{
    static int const value = boost_ext::mpl::
         if_< is_callable_mem_fun<T>, sake::int_tag<2> >::type::template
    else_if < ::sake_sign_private::is_callable< void ( T ) >, sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
};

template< class T >
class dispatch2_impl
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval<T>().sign(),
        result_of::default_impl::sign_result_types,
        nominal_type
    );
public:
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        sake::sign_t,
        nominal_type
    >::type type;
};

template< class T >
struct dispatch< T, void, 2 >
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_cv_or<T>::value));
    typedef typename dispatch2_impl<T>::type type;
};

template< class T >
struct dispatch< T&, void, 2 >
{
    typedef typename dispatch2_impl< T& >::type maybe_type;
public:
    typedef typename boost::mpl::eval_if_c<
        boost::is_void< maybe_type >::value,
        result_of::sign< typename boost::remove_cv<T>::type >,
        boost::mpl::identity< maybe_type >
    >::type type;
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    template< class T_ >
    static Result apply(SAKE_FWD2_REF( T_ ) x)
    { return static_cast< Result >(sake::forward< T_ >(x).sign()); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : ::sake_sign_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename sake::result_of::cmp< T, sake::zero_t >::type type;
    template< class T_ >
    static type apply(SAKE_FWD2_REF( T_ ) x)
    { return sake::cmp(sake::forward< T_ >(x), sake::zero); }
};

} // namespace sign_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIGN_HPP
