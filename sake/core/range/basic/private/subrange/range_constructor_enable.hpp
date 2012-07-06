/*******************************************************************************
 * sake/core/range/basic/private/subrange/range_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RANGE_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RANGE_CONSTRUCTOR_ENABLE_HPP

#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace range
{

namespace basic
{

namespace subrange_private
{

template< class I, class R >
struct range_constructor_enable_iterator
    : boost_ext::is_convertible<
          typename sake::range_iterator<
              R, typename sake::iterator_introversal<I>::type >::type,
          I
      >
{ };

template< class This, class R >
struct range_constructor_enable
    : boost_ext::mpl::and3<
          boost_ext::not_is_base_of_sans_qualifiers< This, R >,
          boost::mpl::not_< boost_ext::is_convertible<
              R, typename This::iterator > >,
          range_constructor_enable_iterator<
              typename This::iterator,
              typename boost_ext::remove_reference<R>::type
          >
      >
{ };

template< class This, class R >
struct range_constructor_enabler
    : boost::enable_if_c< range_constructor_enable<
          This, typename boost_ext::remove_rvalue_reference<R>::type >::value >
{ };

} // namespace subrange_private

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_RANGE_CONSTRUCTOR_ENABLE_HPP
