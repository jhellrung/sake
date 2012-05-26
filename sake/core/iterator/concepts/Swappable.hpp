/*******************************************************************************
 * sake/core/iterator/concepts/Swappable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Swappable<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/config.hpp>
#include <sake/core/concepts/CopyConstructible.hpp>
#include <sake/core/iterator/iter_swap.hpp>

namespace sake
{

namespace iterator_concepts
{

#if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif // #if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)

template< class I >
struct Swappable
    : sake::concepts::CopyConstructible<I>
{
    BOOST_CONCEPT_USAGE( Swappable )
    {
        sake::iter_swap(i, j);
    }

private:
    I const i;
    I const j;
};

#if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)
#pragma GCC diagnostic pop
#endif // #if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP
