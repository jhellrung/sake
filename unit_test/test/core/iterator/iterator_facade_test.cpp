/*******************************************************************************
 * unit_test/test/core/iterator/iterator_facade_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/iterator/concepts/Bidirectional.hpp>
#include <sake/core/iterator/concepts/Forward.hpp>
#include <sake/core/iterator/concepts/Incrementable.hpp>
#include <sake/core/iterator/concepts/RandomAccess.hpp>
#include <sake/core/iterator/concepts/Readable.hpp>
#include <sake/core/iterator/concepts/SinglePass.hpp>
#include <sake/core/iterator/concepts/WritableLvalue.hpp>
#include <sake/core/iterator/facade.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/utility/using_typedef.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace
{

template< class T, class Traversal >
class iterator;

template< class T, class Traversal >
struct iterator_facade
{
    typedef sake::iterator_facade<
        iterator< T, Traversal >,
        boost::mpl::map3<
            sake::iterator_keyword::value<T>,
            sake::iterator_keyword::reference<
                typename boost::mpl::eval_if_c<
                    boost::is_const<T>::value,
                    boost::remove_const<T>,
                    boost::mpl::identity< T& >
                >::type
            >,
            sake::iterator_keyword::traversal< Traversal >
        >
    > type;
};

template< class T >
class iterator< T, boost::incrementable_traversal_tag >
    : public iterator_facade< T, boost::incrementable_traversal_tag >::type
{
    typedef typename iterator_facade<
        T, boost::incrementable_traversal_tag
    >::type iterator_facade_;
public:
    SAKE_USING_TYPEDEF( typename iterator_facade_, reference );
    T* p;
    explicit iterator(T& x) : p(&x) { }
private:
    friend class sake::iterator_core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
};

template< class T >
class iterator< T, boost::single_pass_traversal_tag >
    : public iterator_facade< T, boost::single_pass_traversal_tag >::type
{
    typedef typename iterator_facade<
        T, boost::single_pass_traversal_tag
    >::type iterator_facade_;
public:
    SAKE_USING_TYPEDEF( typename iterator_facade_, reference );
    T* p;
    explicit iterator(T& x) : p(&x) { }
private:
    friend class sake::iterator_core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
    bool derived_equal(iterator other) const { return p == other.p; }
};

} // namespace

void iterator_facade_test(sake::test::environment& env)
{
    {
        typedef iterator< int const, boost::incrementable_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Incrementable< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Readable< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        ++i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
    }
    {
        typedef iterator< int, boost::incrementable_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Incrementable< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::WritableLvalue< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        ++i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        *i = 1;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env,  x, ==, 1 );
    }
    {
        typedef iterator< int const, boost::single_pass_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::SinglePass< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::Readable< type >));
        int x = 0, y = 1;
        type i(x), j(y);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *j, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, i, !=, j );
        SAKE_TEST_CHECK( env, !(i == j) );
        j = i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *j, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK( env, !(i != j) );
    }
    {
        typedef iterator< int, boost::single_pass_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::SinglePass< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator_concepts::WritableLvalue< type >));
        int x = 0, y = 1;
        type i(x), j(y);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *j, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, i, !=, j );
        SAKE_TEST_CHECK( env, !(i == j) );
        j = i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, *j, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK( env, !(i != j) );
        *i = 1;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, *j, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env,  x, ==, 1 );
    }
    {
        typedef iterator< int, boost::forward_traversal_tag > type;
        
    }
}

} // namespace sake_unit_test
