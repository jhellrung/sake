/*******************************************************************************
 * sake/core/iterator/private/introversal_adaptor/traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_TRAITS_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_TRAITS_HPP

#include <boost/mpl/map/map10.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/introversal_adaptor_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

namespace introversal_adaptor_private
{

template< class I, bool ExplicitBegin, bool ExplicitEnd, class IntroversalMask >
struct traits
{
    typedef sake::iterator::introversal_adaptor<
        I, ExplicitBegin, ExplicitEnd, IntroversalMask > introversal_adaptor_;

    typedef typename sake::iterator_introversal<I>::type base_introversal;

    static unsigned int const begin_mask_value =
        sake::begin_access_introversal_tag::value;
    static unsigned int const end_mask_value =
        sake::end_access_introversal_tag::value;
    static unsigned int const base_value = base_introversal::value;
    static unsigned int const mask_value = IntroversalMask::value;

    BOOST_STATIC_ASSERT((
        !ExplicitBegin || 0 != (~mask_value & begin_mask_value)));
    BOOST_STATIC_ASSERT((
        !ExplicitEnd || 0 != (~mask_value & end_mask_value)));

    static unsigned int const implicit_value = base_value & mask_value;
    static unsigned int const explicit_value =
        ExplicitBegin * begin_mask_value | ExplicitEnd * end_mask_value;
    static unsigned int const introversal_value = implicit_value | explicit_value;
    typedef sake::introversal_tag_c< introversal_value > introversal;
    typedef sake::iterator::adaptor<
        introversal_adaptor_, I,
        boost::mpl::map1<
            sake::iterator::keyword::introversal< introversal >
        >
    > adaptor_;

    typedef typename sake::iterator_relax<I>::type null_base_type;
};

} // namespace introversal_adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_TRAITS_HPP
