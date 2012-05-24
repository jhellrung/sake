/*******************************************************************************
 * sake/core/iterator/private/facade/explicit_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_EXPLICIT_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_EXPLICIT_CONSTRUCTOR_ENABLE_HPP

#include <boost/mpl/has_key.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template<
    class D, class P, class T,
    bool = boost::mpl::has_key<
        P, sake::iterator_keyword::tag::chained_base >::value
>
struct explicit_constructor_enable_dispatch;

template< class D, class P, class T >
struct explicit_constructor_enable_dispatch< D, P, T, false >
    : boost::false_type
{ };

template< class D, class P, class T >
struct explicit_constructor_enable_dispatch< D, P, T, true >
    : boost_ext::mpl::and2<
          boost::mpl::not_< boost_ext::is_base_of_sans_qualifiers<
              sake::iterator_facade_adl::iterator_facade<D,P>, T > >,
          sake::value_constructor_enable<
              typename boost_ext::mpl::at<
                  P, sake::iterator_keyword::tag::chained_base >::type,
              T
          >
      >
{ };

template< class D, class P, class T >
struct explicit_constructor_enabler
    : boost::enable_if_c< private_::explicit_constructor_enable_dispatch<
          D, P, typename boost_ext::remove_rvalue_reference<T>::type
      >::value >
{ };

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_EXPLICIT_CONSTRUCTOR_ENABLE_HPP
