/*******************************************************************************
 * sake/boost_ext/type_traits/common_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::common_type< T0, T1 >
 *
 * Inspired by
 *     http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm#common_type
 *
 * common_type< T0, T1 >::type is the type of the expression
 *     declval< bool >() ? declval< T0 >() : declval< T1 >()
 * Note that this evaluates to void if one or both of T0 and T1 is void.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP

#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/make_signed.hpp>
#include <sake/boost_ext/type_traits/make_unsigned.hpp>
#include <sake/boost_ext/type_traits/propagate_cv.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/typeof.hpp>
#include <sake/core/expr_traits/is_rvalue.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace boost_ext
{

template< class T0, class T1 >
struct common_type;

namespace common_type_private
{

template< class T0, class T1 >
struct dispatch_on_void;

template<
    class T0, class T1,
    bool = SAKE_EXPR_IS_RVALUE(
        sake::declval< bool >() ?
        sake::declval< T0 >() :
        sake::declval< T1 >()
    )
>
struct dispatch_on_rvalue_result;

template<
    class T0, class T1,
    class U0 = typename boost_ext::remove_reference< T0 >::type,
    class U1 = typename boost_ext::remove_reference< T1 >::type
>
struct lvalue_result_impl;

template< class T0, class T1, class Candidates >
struct deduce_from_candidates;

template< class T0, class T1 >
struct dispatch_on_void
    : dispatch_on_rvalue_result< T0, T1 >
{ };

template< class T > struct dispatch_on_void< T, void > { typedef void type; };
template< class T > struct dispatch_on_void< void, T > { typedef void type; };
template<> struct dispatch_on_void< void, void > { typedef void type; };

template<
    class T0, class T1,
    bool = boost_ext::is_integral_or_enum< T0 >::value
        && boost_ext::is_integral_or_enum< T1 >::value
>
struct nominal_candidates_dispatch;

template< class T0, class T1 >
struct nominal_candidates_dispatch< T0, T1, false >
{ typedef boost::mpl::vector2< T0, T1 > type; };

template< class T0, class T1 >
struct nominal_candidates_dispatch< T0*, T1*, false >
{
    typedef boost::mpl::vector4<
        T0*, T1*,
        typename boost_ext::propagate_cv< T1, T0 >::type *,
        typename boost_ext::propagate_cv< T0, T1 >::type *
    > type;
};

template< class T0, class T1 >
struct nominal_candidates_dispatch< T0, T1, true >
{
    typedef boost::mpl::vector8<
        typename boost_ext::make_unsigned< T0 >::type,
        typename boost_ext::make_unsigned< T1 >::type,
        typename boost_ext::make_signed< T0 >::type,
        typename boost_ext::make_signed< T1 >::type,
        T0, T1, unsigned int, int
    > type;
};

template< class T0, class T1 >
struct nominal_candidates
    : nominal_candidates_dispatch<
          typename boost_ext::remove_qualifiers< T0 >::type,
          typename boost_ext::remove_qualifiers< T1 >::type
      >
{ };

template< class T0, class T1 >
struct dispatch_on_rvalue_result< T0, T1, true >
    : deduce_from_candidates<
          T0, T1,
          typename nominal_candidates< T0, T1 >::type
      >
{ };

template< class T0, class T1 >
struct dispatch_on_rvalue_result< T0, T1, false >
    : lvalue_result_impl< T0, T1 >
{ };

template< class T0, class T1, class U0, class U1 >
struct lvalue_result_impl
    : deduce_from_candidates<
          T0, T1,
          boost::mpl::vector4<
              U0&, U1&,
              typename boost_ext::propagate_cv< U1, U0 >::type &,
              typename boost_ext::propagate_cv< U0, U1 >::type &
          >
      >
{ };

template< class T0, class T1, class Candidates >
struct deduce_from_candidates
{
    SAKE_EXPR_TYPEOF_TYPEDEF(
        typename sake::declval< bool >() ? sake::declval< T0 >() : sake::declval< T1 >(),
        Candidates,
        type
    );
    BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY(
        typename boost_ext::mpl::curry_quote2< boost::is_same >::apply< type >::type,
        sake::declval< bool >() ? sake::declval< T0 >() : sake::declval< T1 >()
    ) );
};

} // namespace common_type_private

template< class T0, class T1 >
struct common_type
    : common_type_private::dispatch_on_void<
          typename boost::remove_cv<
              typename boost_ext::remove_rvalue_reference< T0 >::type
          >::type,
          typename boost::remove_cv<
              typename boost_ext::remove_rvalue_reference< T1 >::type
          >::type
      >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP
