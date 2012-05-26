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

#include <sake/core/config.hpp>

namespace sake
{

namespace concepts
{

#if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif // #if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)

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

#if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)
#pragma GCC diagnostic pop
#endif // #if SAKE_GNUC_VERSION && SAKE_GNUC_VERSION >= SAKE_GNUC_VERSION_OF(4,6,3)

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_RELATIONAL_COMPARABLE_HPP
