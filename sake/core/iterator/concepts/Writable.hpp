/*******************************************************************************
 * sake/core/iterator/concepts/Writable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Writable<
 *     I, T = sake::iterator_value<I>::type
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/concepts/CopyConstructible.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator_concepts
{

template<
    class I,
    class T = typename sake::iterator_value<I>::type
>
struct Writable
    : sake::concepts::CopyConstructible<I>
{
    BOOST_CONCEPT_USAGE( Writable )
    {
        *i = x;
    }

private:
    I const i;
    T x;
};

template< class I >
struct Writable< I, void >
    : sake::concepts::CopyConstructible<I>
{ };

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP
