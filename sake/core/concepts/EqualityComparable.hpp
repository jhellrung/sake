/*******************************************************************************
 * sake/core/concepts/EqualityComparable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::EqualityComparable< T, U = T, Result = bool >
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP
#define SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/config.hpp>

namespace sake
{

namespace concepts
{

template< class T, class U = T, class Result = bool >
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

    void assert_result(Result);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( EqualityComparable )
};

template< class T, class U >
struct EqualityComparable< T, U, void >
{
    BOOST_CONCEPT_USAGE( EqualityComparable )
    {
        x == y;
        x != y;
        y == x;
        y != x;
    }

private:
    T const x;
    U const y;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( EqualityComparable )
};

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_EQUALITY_COMPARABLE_HPP
