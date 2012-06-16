/*******************************************************************************
 * sake/core/concepts/Dereferenceable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::Dereferenceable< T, Result = void >
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_DEREFERENCEABLE_HPP
#define SAKE_CORE_CONCEPTS_DEREFERENCEABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/config.hpp>

namespace sake
{

namespace concepts
{

template< class T, class Result = void >
struct Dereferenceable
{
    BOOST_CONCEPT_USAGE( Dereferenceable )
    {
        assert_result(*x);
    }

private:
    T x;

    void assert_result(Result);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Dereferenceable )
};

template< class T >
struct Dereferenceable< T, void >
{
    BOOST_CONCEPT_USAGE( Dereferenceable )
    {
        static_cast<void>(*x);
    }

private:
    T x;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Dereferenceable )
};

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_DEREFERENCEABLE_HPP
