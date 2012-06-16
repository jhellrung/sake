/*******************************************************************************
 * unit_test/test/core/iterator/introversal_adaptor_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/introversal_adaptor.hpp>
#include <sake/core/iterator/traits.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace iterator
{

namespace
{

} // namespace

void introversal_adaptor_test(sake::test::environment& env)
{
    {
        typedef sake::iterator::introversal_adaptor< int*, false, false > type;
        BOOST_STATIC_ASSERT((sake::boost_ext::is_convertible<
            sake::iterator_traversal< type >::type,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost::is_same<
            sake::iterator_introversal< type >::type,
            sake::null_introversal_tag
        >::value));
        BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< type >));
    }
    {
        typedef sake::iterator::introversal_adaptor< int*, true, false > type;
        BOOST_STATIC_ASSERT((sake::boost_ext::is_convertible<
            sake::iterator_traversal< type >::type,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost::is_same<
            sake::iterator_introversal< type >::type,
            sake::begin_access_introversal_tag
        >::value));
        BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< type >));
        int x = 0, y = 1;
        type i(&y, &x, sake::_end);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, *i.at(sake::_begin), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::null_introversal_tag()), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::begin_detect_introversal_tag()), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::begin_access_introversal_tag()), ==, 0 );
        i.begin_ip();
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
    }
    {
        typedef sake::iterator::introversal_adaptor< int*, false, true > type;
        BOOST_STATIC_ASSERT((sake::boost_ext::is_convertible<
            sake::iterator_traversal< type >::type,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost::is_same<
            sake::iterator_introversal< type >::type,
            sake::end_access_introversal_tag
        >::value));
        BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< type >));
        int y = 1, z = 2;
        type i(&y, sake::_begin, &z);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, *i.at(sake::_end), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::null_introversal_tag()), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::end_detect_introversal_tag()), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::end_access_introversal_tag()), ==, 2 );
        i.end_ip();
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 2 );
    }
    {
        typedef sake::iterator::introversal_adaptor< int*, true, true > type;
        BOOST_STATIC_ASSERT((sake::boost_ext::is_convertible<
            sake::iterator_traversal< type >::type,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost::is_same<
            sake::iterator_introversal< type >::type,
            sake::begin_access_end_access_introversal_tag
        >::value));
        BOOST_CONCEPT_ASSERT((sake::concepts::Iterator< type >));
        int x = 0, y = 1, z = 2;
        type i(&y, &x, &z);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, *i.at(sake::_begin), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::null_introversal_tag()), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::begin_detect_introversal_tag()), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.begin(sake::begin_access_introversal_tag()), ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *i.at(sake::_end), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::null_introversal_tag()), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::end_detect_introversal_tag()), ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, *i.end(sake::end_access_introversal_tag()), ==, 2 );
        i.begin_ip();
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        i.end_ip();
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 2 );
    }
}

} // namespace iterator

} // namespace sake_unit_test
