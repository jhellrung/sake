/*******************************************************************************
 * sake/core/iterator/concepts/Writable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::Writable<
 *     I, T = sake::iterator_value<I>::type
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template<
    class I,
    class T = typename sake::iterator_value<I>::type
>
struct Writable
    : sake::iterator::concepts::private_::Base<I>
{
    BOOST_CONCEPT_USAGE( Writable )
    {
        *i = x;
    }

private:
    I const i;
    T x;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Writable )
};

template< class I >
struct Writable< I, void >
    : sake::iterator::concepts::private_::Base<I>
{ };

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_HPP
