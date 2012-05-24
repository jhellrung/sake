/*******************************************************************************
 * unit_test/test/core/utility/is_template_base_of_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>

#include <sake/core/utility/is_template_base_of.hpp>

#include <sake/test/environment.hpp>

namespace sake_unit_test
{

namespace
{

template< class T >
struct B1
{ };

BOOST_STATIC_ASSERT((!sake::is_template_base_of1< B1, void >::value));
BOOST_STATIC_ASSERT((!sake::is_template_base_of1< B1, int >::value));

struct X
{ };

BOOST_STATIC_ASSERT((!sake::is_template_base_of1< B1, X >::value));

template< class T >
struct D1 : B1<T>
{ };

BOOST_STATIC_ASSERT((sake::is_template_base_of1< B1, D1< void > >::value));
BOOST_STATIC_ASSERT((sake::is_template_base_of1< B1, D1< int > >::value));

template< class T0, class T1 >
struct B2 : T1
{ };

BOOST_STATIC_ASSERT((!sake::is_template_base_of2< B2, void >::value));
BOOST_STATIC_ASSERT((!sake::is_template_base_of2< B2, int >::value));
BOOST_STATIC_ASSERT((!sake::is_template_base_of2< B2, X >::value));

template< class T0, class T1 >
struct D2 : B2< T0, T1 >
{ };

BOOST_STATIC_ASSERT((sake::is_template_base_of2< B2, D2< void, X > >::value));
BOOST_STATIC_ASSERT((sake::is_template_base_of2< B2, D2< int, X > >::value));
BOOST_STATIC_ASSERT((sake::is_template_base_of2< B2, D2< X, X > >::value));
BOOST_STATIC_ASSERT((sake::is_template_base_of2<
    B2, D2< void, boost::true_type >,
    boost::mpl::quote1< boost::mpl::identity >
>::value));
BOOST_STATIC_ASSERT((!sake::is_template_base_of2<
    B2, D2< void, boost::false_type >,
    boost::mpl::quote1< boost::mpl::identity >
>::value));

} // namespace

void is_template_base_of_test(sake::test::environment& /*env*/)
{ }

} // namespace sake_unit_test
