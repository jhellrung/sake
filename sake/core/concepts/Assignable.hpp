/*******************************************************************************
 * sake/core/concepts/Assignable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::Assignable< To, From >
 * struct concepts::Assignable< To ( From ) >
 ******************************************************************************/

#ifndef SAKE_CORE_CONCEPTS_ASSIGNABLE_HPP
#define SAKE_CORE_CONCEPTS_ASSIGNABLE_HPP

#include <boost/concept/usage.hpp>

#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace concepts
{

template< class To, class From = void >
struct Assignable
{
    BOOST_CONCEPT_USAGE( Assignable )
    {
        assert_result(to = sake::declval< From >());
    }

private:
    To to;

    void assert_result(To&);
};

template< class To, class From >
struct Assignable< To ( From ) >
    : sake::concepts::Assignable< To, From >
{ };

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_ASSIGNABLE_HPP
