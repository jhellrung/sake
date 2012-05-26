/*******************************************************************************
 * sake/core/concepts/EqualityComparable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::EqualityComparable< T, U = T >
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP
#define SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP

#include <boost/concept/usage.hpp>

namespace sake
{

namespace concepts
{

template< class T, class U = T >
struct EqualityComparable
{
    BOOST_CONCEPT_USAGE( EqualityComparable )
    {
        assert_result(x == y);
        assert_result(x != y);
        assert_result(y == x);
        assert_result(y != x);
    }

private:
    T const x;
    U const y;

    void assert_result(bool);
};

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP
