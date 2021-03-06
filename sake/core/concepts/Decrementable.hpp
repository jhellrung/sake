/*******************************************************************************
 * sake/core/concepts/Decrementable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::Decrementable<T>
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_DECREMENTABLE_HPP
#define SAKE_CORE_CONCEPTS_DECREMENTABLE_HPP

#include <boost/concept/usage.hpp>

namespace sake
{

namespace concepts
{

template< class T >
struct Decrementable
{
    BOOST_CONCEPT_USAGE( Decrementable )
    {
        assert_result< T& >(--x);
        assert_result<T>(x--);
    }

private:
    T x;

    template< class U >
    void assert_result(U);
};

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_DECREMENTABLE_HPP
