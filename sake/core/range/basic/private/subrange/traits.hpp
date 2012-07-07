/*******************************************************************************
 * sake/core/range/basic/private/subrange/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_TRAITS_HPP
#define SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_TRAITS_HPP

#include <boost/concept/assert.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/basic/fwd.hpp>
#include <sake/core/range/facade.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/keyword.hpp>

namespace sake
{

namespace range
{

namespace basic
{

namespace subrange_private
{

template< class I, class N = void >
struct traits
{
    BOOST_CONCEPT_ASSERT((sake::concepts::Iterator<I>));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::single_pass_traversal_tag
    >::value));
    BOOST_STATIC_ASSERT((boost::mpl::contains<
        boost::mpl::vector4<
            sake::null_introterminal_tag,
            sake::begin_access_introterminal_tag,
            sake::end_access_introterminal_tag,
            sake::begin_access_end_access_introterminal_tag
        >,
        typename sake::iterator_introterminal<I>::type
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost::is_void<N>,
        sake::has_isc_value<N>
    >::value));

    typedef typename sake::iterator_relax<I>::type null_base_type;
    typedef sake::range::facade<
        sake::range::basic::subrange<I,N>,
        boost::mpl::map1<
            sake::range::keyword::iterator< null_base_type >
        >
    > facade_;
};

} // namespace subrange_private

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_TRAITS_HPP
