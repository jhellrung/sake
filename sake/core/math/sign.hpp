/*******************************************************************************
 * sake/core/math/sign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * sign(T const & x) -> result_of::sign<T>::type
 * struct functional::sign
 *
 * struct result_of::sign<T>
 * struct result_of::extension::sign< T, Enable = void >
 * struct result_of::default_impl::sign<T>
 *
 * Teturns the sign of its argument (either a sake::sign_t or, in the event
 * that the sign could be indeterminate, a sake::fuzzy_sign_t).
 *
 * sake::sign(T const &) is implemented in terms of
 * - T::sign(), if available; else
 * - sign(T const &) (unqualified, hence subject to ADL), if available; else
 * - comparison with sake::zero (via sake::cmp).
 *
 * The default implementation of the result_of::sign metafunction is evaluated
 * based on the result of the first available implementation above and is able
 * to detect result types convertible to int, sake::sign_t, and
 * sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIGN_HPP
#define SAKE_CORE_MATH_SIGN_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/cmp.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/sign_fwd.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/declval.hpp>
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
    class Result = typename result_of::sign<T>::type,
    unsigned int = dispatch_index<T>::value
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
    : extension::sign<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

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

typedef boost::mpl::vector3<
    int,
    sake::sign_t,
    sake::fuzzy_sign_t
> sign_result_types;

template< class T >
struct sign
    : sign_private::dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          void
      >
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
    SAKE_RESULT_FROM_METAFUNCTION( result_of::sign, 1 )

    template< class T >
    typename result_of::sign<T>::type
    operator()(T const & x) const
    { return sign_private::dispatch<T>::apply(x); }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace sign_adl_barrier
{ functional::sign const sign = { }; }
using namespace sign_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
functional::sign const sign = { };
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
    static Result apply(T const & x)
    { return static_cast< Result >(sign(x)); }
};

template< class T >
struct adl< T, void >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        typename sign(::sake::declcref<T>()),
        ::sake::result_of::default_impl::sign_result_types,
        nominal_type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< nominal_type >::value));
    typedef typename boost::mpl::if_c<
        ::boost::is_same< nominal_type, int >::value,
        ::sake::sign_t,
        nominal_type
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
    static unsigned int const _ =
        (1 << 2) * is_callable_mem_fun< T const & >::value
      | (1 << 1) * sake_sign_private::is_callable< void ( T const & ) >::value
      | (1 << 0);
    static unsigned int const value = sake::static_intlog2_c<_>::value;
};

template< class T >
struct dispatch< T, void, 2 >
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        typename sake::declcref<T>().sign(),
        result_of::default_impl::sign_result_types,
        nominal_type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< nominal_type >::value));
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        sake::sign_t,
        nominal_type
    >::type type;
};

template< class T, class Result >
struct dispatch< T, Result, 2 >
{
    static Result apply(T const & x)
    { return static_cast< Result >(x.sign()); }
};

template< class T, class Result >
struct dispatch< T, Result, 1 >
    : ::sake_sign_private::adl< T, Result >
{ };

template< class T, class Result >
struct dispatch< T, Result, 0 >
{
    typedef typename sake::result_of::cmp< T, sake::zero_t >::type type;
    static type apply(T const & x)
    { return sake::cmp(x, sake::zero); }
};

} // namespace sign_private

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIGN_HPP
