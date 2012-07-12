/*******************************************************************************
 * sake/core/iterator/concepts/private/IntroterminalBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/EqualityComparable.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/At.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/concepts/private/Difference.hpp>
#include <sake/core/iterator/concepts/private/IntroterminalBase_fwd.hpp>
#include <sake/core/iterator/concepts/private/Relax.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

// Null
template< class I >
struct IntroterminalBase< I, sake::null_introterminal_tag >
    : sake::iterator::concepts::private_::Base<I>,
      sake::iterator::concepts::private_::At<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::null_introterminal_tag >
{ };

// BeginDetect
template< class I >
struct IntroterminalBase< I, sake::begin_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_detect_introterminal_tag >,
      sake::concepts::EqualityComparable< I, sake::begin_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_introterminal_tag
    >::value));
};

// BeginAccess
template< class I >
struct IntroterminalBase< I, sake::begin_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::null_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_detect_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::Difference<
          I, sake::begin_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_introterminal_tag
    >::value));
};

// EndDetect
template< class I >
struct IntroterminalBase< I, sake::end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::end_detect_introterminal_tag >,
      sake::concepts::EqualityComparable< I, sake::end_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::end_detect_introterminal_tag
    >::value));
};

// EndAccess
template< class I >
struct IntroterminalBase< I, sake::end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::end_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::null_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::end_detect_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::Difference<
          I, sake::end_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::end_access_introterminal_tag
    >::value));
};

// BeginDetect + EndDetect
template< class I >
struct IntroterminalBase< I, sake::begin_detect_end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_detect_end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_end_detect_introterminal_tag
    >::value));
};

// BeginAccess + EndDetect
template< class I >
struct IntroterminalBase< I, sake::begin_access_end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_end_detect_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_access_end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_end_detect_introterminal_tag
    >::value));
};

// BeginDetect + EndAccess
template< class I >
struct IntroterminalBase< I, sake::begin_detect_end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_detect_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_detect_end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_detect_end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_end_access_introterminal_tag
    >::value));
};

// BeginAccess + EndAccess
template< class I >
struct IntroterminalBase< I, sake::begin_access_end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_access_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_end_access_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::At<
          I, sake::begin_access_end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::Relax<
          I, sake::begin_access_end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_end_access_introterminal_tag
    >::value));
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP
