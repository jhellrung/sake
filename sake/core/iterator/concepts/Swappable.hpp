/*******************************************************************************
 * sake/core/iterator/concepts/Swappable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::Swappable<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/iter_swap.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template< class I >
struct Swappable
    : sake::iterator::concepts::private_::Base<I>
{
    BOOST_CONCEPT_USAGE( Swappable )
    {
        sake::iter_swap(i, j);
    }

private:
    I const i;
    I const j;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Swappable )
};

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_SWAPPABLE_HPP
