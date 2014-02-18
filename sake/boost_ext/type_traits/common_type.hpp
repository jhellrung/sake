/*******************************************************************************
 * sake/boost_ext/type_traits/common_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::common_type< T0, T1 >
 * struct boost_ext::has_common_type< T0, T1 >
 *
 * Inspired by
 *   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2661.htm#common_type
 *
 * common_type< T0, T1 >::type is the type of the expression
 *   declval< bool >() ? declval< T0 >() : declval< T1 >()
 * based on ISO/IEC 14882:2003(E) 5.16:
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP

#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/decay.hpp>
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
#include <sake/core/introspection/has_type_type.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace boost_ext
{

template< class T0, class T1 >
struct common_type;

template< class T0, class T1 >
struct has_common_type
  : sake::has_type_type< boost_ext::common_type< T0, T1 > >
{ };

namespace common_type_private
{

template< class T0, class T1 >
struct dispatch_on_void;

#if 0

template< class From, class To >
class convert_helper
{
  typedef typename boost_ext::
    remove_reference< From >::type noref_from_type;
  typedef typename boost_ext::
    remove_reference< To >::type noref_to_type;
  typedef typename boost_ext::
    remove_cv< noref_from_type >::type noqual_from_type;
  typedef typename boost_ext::
    remove_cv< noref_to_type >::type noqual_to_type;
  typedef typename boost_ext::mpl::
  eval_if<
    boost_ext::mpl::and3<
      boost::is_class< noqual_from_type >,
      boost::is_class< noqual_to_type >,
      boost_ext::mpl::or2<
        boost::is_base_of< noqual_from_type, noqual_to_type >,
        boost::is_base_of< noqual_to_type, noqual_from_type >
      >
    >,
    boost::mpl::if_<
      boost_ext::mpl::and2<
        boost::is_base_of< noqual_to_type, noqual_from_type >,
        boost_ext::has_cv_qual_ge< noqual_to_type, noqual_from_type >
      >,
      boost::mpl::pair< boost::true_type, noref_to_type >,
      boost::mpl::pair< boost::false_type, void >
    >
  >::type::template
  else_if<
    boost_ext::is_convertible< From, noqual_to_type >,
    boost::mpl::pair< boost::true_type, noqual_to_type >
  >::type::template
  else_<
    boost::mpl::pair< boost::false_type, void >
  >::type type;
public:
  static bool const value = type::first_type::value;
  typedef typename type::second_type result_type;
};

template< class From, class To >
struct convert< From, To & >
{
  typedef typename boost_ext::remove_qualifiers< From >::type noqual_from_type;
  typedef typename boost_ext::remove_cv< To >::type noqual_to_type;
  typedef typename boost::mpl::eval_if_c<
    sake::is_convertible_wnrbt< From, To & >::value,
    boost::mpl::identity< boost::mpl::pair< boost::true_type, To & > >,
    convert_helper< From, To & >
  >::type type;
  static bool const value = type::first_type::value;
  typedef typename type::second_type result_type;
};

#endif

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

template< class T >
struct dispatch_on_void< T, void >
  : boost::decay< typename boost_ext::remove_qualifiers<T>::type >
{ };

template< class T >
struct dispatch_on_void< void, T >
  : boost::decay< typename boost_ext::remove_qualifiers<T>::type >
{ };

template<>
struct dispatch_on_void< void, void >
{ typedef void type; };

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
      T0, T1, typename nominal_candidates< T0, T1 >::type >
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
    typename sake::declval< bool >() ?
      sake::declval< T0 >() : sake::declval< T1 >(),
    Candidates,
    type
  );
  BOOST_STATIC_ASSERT((!boost::is_void< type >::value));
private:
  // MSVC9 workaround.
  static bool const is_same_ = SAKE_EXPR_APPLY(
    typename boost_ext::mpl::curry_quote2<
      boost::is_same >::apply< type >::type,
    sake::declval< bool >() ? sake::declval< T0 >() : sake::declval< T1 >()
  );
  BOOST_STATIC_ASSERT((is_same_));
};

} // namespace common_type_private

template< class T0, class T1 >
struct common_type
  : common_type_private::dispatch_on_void<
      typename boost::remove_cv<
        typename boost_ext::remove_rvalue_reference< T0 >::type >::type,
      typename boost::remove_cv<
        typename boost_ext::remove_rvalue_reference< T1 >::type >::type
    >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_COMMON_TYPE_HPP
