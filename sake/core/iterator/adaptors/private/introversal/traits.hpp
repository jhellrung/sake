/*******************************************************************************
 * sake/core/iterator/adaptors/private/introversal/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_TRAITS_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_TRAITS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set/set0.hpp>
#include <boost/mpl/set/set10.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/adaptor_fwd.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/conversion_operators/base.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace introversal_private
{

template< class I >
struct relax_types
    : boost::mpl::transform<
          boost::mpl::filter_view<
              boost::mpl::vector9<
                  sake::null_introversal_tag,
                  sake::begin_detect_introversal_tag,
                  sake::begin_access_introversal_tag,
                  sake::end_detect_introversal_tag,
                  sake::end_access_introversal_tag,
                  sake::begin_detect_end_detect_introversal_tag,
                  sake::begin_access_end_detect_introversal_tag,
                  sake::begin_detect_end_access_introversal_tag,
                  sake::begin_access_end_access_introversal_tag
              >,
              typename boost_ext::mpl::curry_quote2<
                  boost_ext::is_convertible
              >::apply< typename sake::iterator_introversal<I>::type >::type
          >,
          typename boost_ext::mpl::curry_quote2<
              sake::iterator_relax >::apply<I>::type,
          boost::mpl::inserter<
              boost::mpl::set0<>,
              boost::mpl::insert< boost::mpl::_1, boost::mpl::_2 >
          >
      >
{ };

template< class I, class Tags, class IntroversalMask >
struct traits
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Tags >::value));
    typedef sake::iterator::adaptors::introversal<
        I, Tags, IntroversalMask > this_type;

    static unsigned int const begin_mask_value =
        sake::begin_access_introversal_tag::value;
    static unsigned int const end_mask_value =
        sake::end_access_introversal_tag::value;

    typedef typename sake::iterator_introversal<I>::type base_introversal;
    static unsigned int const base_value = base_introversal::value;
    static unsigned int const mask_value = IntroversalMask::value;

    static bool const explicit_begin =
        boost::mpl::has_key< Tags, sake::begin_tag >::value;
    static bool const explicit_end =
        boost::mpl::has_key< Tags, sake::end_tag >::value;
    BOOST_STATIC_ASSERT((
        !explicit_begin || 0 != (~mask_value & begin_mask_value)));
    BOOST_STATIC_ASSERT((
        !explicit_end || 0 != (~mask_value & end_mask_value)));

    static unsigned int const implicit_value = base_value & mask_value;
    static unsigned int const explicit_value =
        explicit_begin * begin_mask_value | explicit_end * end_mask_value;
    static unsigned int const introversal_value =
        implicit_value | explicit_value;
    typedef sake::introversal_tag_c< introversal_value > introversal;

    typedef sake::conversion_operators::base<
        this_type, typename introversal_private::relax_types<I>::type
    > conversion_operators_base_;

    typedef sake::iterator::adaptor<
        this_type, I,
        boost::mpl::map2<
            sake::iterator::keyword::introversal< introversal >,
            sake::iterator::keyword::chained_base< conversion_operators_base_ >
        >
    > adaptor_;

    typedef typename sake::iterator_relax<I>::type null_base_type;
};

} // namespace introversal_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_TRAITS_HPP
