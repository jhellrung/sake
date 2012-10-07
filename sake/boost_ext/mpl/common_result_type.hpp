/*******************************************************************************
 * sake/boost_ext/mpl/common_result_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::common_result_type< Sequence >
 * struct boost_ext::mpl::has_common_result_type< Sequence >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP
#define SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>

#include <sake/boost_ext/type_traits/common_result_type.hpp>

#include <sake/core/introspection/has_type_type.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Sequence >
struct common_result_type;

template< class Sequence >
struct has_common_result_type
  : sake::has_type_type< boost_ext::mpl::common_result_type< Sequence > >
{ };

namespace common_result_type_private
{

template< class I, class E >
struct dispatch;

template< class I, class E, class T >
struct iterate;

template<
  class I, class E, class Common,
  bool = sake::has_type_type< Common >::value
>
struct iterate_helper;

template< class I, class E >
struct dispatch
  : common_result_type_private::iterate<
      typename boost::mpl::next<I>::type, E,
      typename boost::mpl::deref<I>::type
    >
{ };

template< class E >
struct dispatch<E,E>
{ };

template< class I, class E, class T >
struct iterate
  : iterate_helper<
      I, E,
      boost_ext::common_result_type< T, typename boost::mpl::deref<I>::type >
    >
{ };

template< class E, class T >
struct iterate<E,E,T>
{ typedef T type; };

template< class I, class E, class Common >
struct iterate_helper< I, E, Common, false >
{ };

template< class I, class E, class Common >
struct iterate_helper< I, E, Common, true >
  : common_result_type_private::iterate<
      typename boost::mpl::next<I>::type, E, typename Common::type >
{ };

} // namespace common_result_type_private

template< class Sequence >
struct common_result_type
  : common_result_type_private::dispatch<
      typename boost::mpl::begin< Sequence >::type,
      typename boost::mpl::end< Sequence >::type
    >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_COMMON_RESULT_TYPE_HPP
