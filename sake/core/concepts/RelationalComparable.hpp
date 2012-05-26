/*******************************************************************************
 * sake/core/concepts/RelationalComparable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::RelationalComparable< T, U = T >
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_RELATIONAL_COMPARABLE_HPP
#define SAKE_CORE_CONCEPTS_RELATIONAL_COMPARABLE_HPP

#include <boost/concept/usage.hpp>

namespace sake
{

namespace concepts
{

template< class T, class U = T >
struct RelationalComparable
{
    BOOST_CONCEPT_USAGE( RelationalComparable )
    {
        assert_result(x <  y);
        assert_result(x >  y);
        assert_result(x <= y);
        assert_result(x >= y);
        assert_result(y <  x);
        assert_result(y >  x);
        assert_result(y <= x);
        assert_result(y >= x);
    }

private:
    T const x;
    U const y;

    void assert_result(bool);
};

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_RELATIONAL_COMPARABLE_HPP
