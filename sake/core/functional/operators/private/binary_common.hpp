/*******************************************************************************
 * sake/core/functional/operators/private/binary_common.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP

#include <cstddef>

#include <boost/logic/tribool.hpp>
#include <boost/mpl/vector/vector20.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/has_qualifier.hpp>
#include <sake/boost_ext/type_traits/make_signed.hpp>
#include <sake/boost_ext/type_traits/make_unsigned.hpp>

#include <sake/core/introspection/has_type_type.hpp>
#include <sake/core/math/indeterminate_fwd.hpp>

namespace sake
{

namespace operators
{

namespace result_of
{

namespace default_impl
{

template<
  class T0, class T1,
  class Common = boost_ext::common_result_type< T0, T1 >,
  bool = sake::has_type_type< Common >::value
>
struct binary_result_types;

template< class T0, class T1, class Common >
struct binary_result_types< T0, T1, Common, false >
{
  BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T0 >::value));
  BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T1 >::value));
  typedef boost::mpl::vector8<
    T0&, // op=, op?=; op<<, op>> (for std::ios)
    T0, T1, // op-, op+ (for pointers)
    std::ptrdiff_t, // op- (for pointers)
    bool, // logical
    boost::logic::tribool, // logical
    sake::functional::indeterminate, // logical
    typename boost::integral_promotion< T0 >::type // shift
  > type;
};

template< class T0, class T1, class Common >
struct binary_result_types< T0, T1, Common, true >
{
  BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T0 >::value));
  BOOST_STATIC_ASSERT((!boost_ext::has_qualifier< T1 >::value));
  typedef boost::mpl::vector11<
    typename Common::type,
    T0&, // op=, op?=; op<<, op>> (for std::ios)
    T0, T1, // op-, op+ (for pointers)
    std::ptrdiff_t, // op- (for pointers)
    bool, // logical
    boost::logic::tribool, // logical
    sake::functional::indeterminate, // logical
    typename boost::integral_promotion< T0 >::type, // shift
    typename boost_ext::make_signed< typename Common::type >::type,
    typename boost_ext::make_unsigned< typename Common::type >::type
  > type;
};

} // namespace default_impl

} // namespace result_of

} // namespace operators

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_PRIVATE_BINARY_COMMON_HPP
