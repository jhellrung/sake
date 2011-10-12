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
 * sign returns the sign of its argument (either a sake::sign_t or, in the event
 * that the sign could be indeterminate, a sake::fuzzy_sign_t).  It
 * automatically uses ADL to find overloads of sign, falling back to a default
 * implementation if ADL fails.
 *
 * The default implementation of sign(T const &)
 * - forwards to T::sign, if available; else
 * - compares the argument to sake::zero via sake::compare.
 *
 * The default implementation of result_of::sign metafunction is evaluated based
 * on the sign overload found via ADL, and is able to detect result types
 * convertible to either sake::sign_t (including int's) and sake::fuzzy_sign_t.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_SIGN_HPP
#define SAKE_CORE_MATH_SIGN_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/math/compare.hpp>
#include <sake/core/math/fuzzy_sign_t.hpp>
#include <sake/core/math/sign_fwd.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/identity_type.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake_sign_private
{
template< class T >
struct result;
} // namespace sake_sign_private

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable_mem_fun_sign
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME sign
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, 0 )
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

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

template< class T >
struct sign
    : ::sake_sign_private::result<
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

} // namespace default_impl

} // namespace result_of

namespace default_impl
{

namespace sign_private
{

template<
    class T,
    bool S     = sake::is_callable_mem_fun_sign< T const &, sake::sign_t ( ) >::value
              || sake::is_callable_mem_fun_sign< T const &, int ( ) >::value,
    bool SorFS = S || sake::is_callable_mem_fun_sign< T const &, sake::fuzzy_sign_t ( ) >::value
>
struct dispatch;

template< class T, bool S >
struct dispatch< T, S, true >
{
    typedef typename boost::mpl::if_c< S, sake::sign_t, sake::fuzzy_sign_t >::type type;
    static type apply(T const & x)
    { return static_cast< type >(x.sign()); }
};

template< class T >
struct dispatch< T, false, false >
{
    typedef typename sake::result_of::compare< T, sake::zero_t >::type type;
    static type apply(T const & x)
    { return sake::compare(x, sake::zero); }
};

} // namespace sign_private

template< class T >
inline typename sign_private::dispatch<T>::type
sign(T const & x)
{ return sign_private::dispatch<T>::apply(x); }

} // namespace default_impl

} // namespace sake

namespace sake_sign_private
{

using ::sake::default_impl::sign;

template< class T >
struct result
{
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF(
        sign(::sake::declcref<T>()),
        SAKE_IDENTITY_TYPE_WRAP((::boost::mpl::vector3<
            int, ::sake::sign_t, ::sake::fuzzy_sign_t >)),
        nominal_type
    );
    typedef typename boost::mpl::if_c<
        boost::is_same< nominal_type, int >::value,
        ::sake::sign_t,
        nominal_type
    >::type type;
};

template< class T >
typename ::sake::result_of::sign<T>::type
impl(T const & x)
{
    typedef typename ::sake::result_of::sign<T>::type result_type;
    return static_cast< result_type >(sign(x));
}

} // namespace sake_sign_private

namespace sake
{

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
    { return ::sake_sign_private::impl(x); }
};

} // namespace functional

functional::sign const sign = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_SIGN_HPP
