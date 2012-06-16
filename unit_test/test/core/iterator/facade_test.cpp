/*******************************************************************************
 * unit_test/test/core/iterator/facade_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/map/map10.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/iterator/categories.hpp>
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

namespace iterator
{

namespace
{

template< class T, class Traversal >
class traversal;

template< class T, class Traversal >
struct traversal_facade
{
    typedef sake::iterator::facade<
        traversal< T, Traversal >,
        boost::mpl::map3<
            sake::iterator::keyword::value<T>,
            sake::iterator::keyword::reference<
                typename boost::mpl::eval_if_c<
                    boost::is_const<T>::value,
                    boost::remove_const<T>,
                    boost::mpl::identity< T& >
                >::type
            >,
            sake::iterator::keyword::traversal< Traversal >
        >
    > type;
};

template< class T >
class traversal< T, boost::incrementable_traversal_tag >
    : public traversal_facade< T, boost::incrementable_traversal_tag >::type
{
    typedef typename traversal_facade<
        T, boost::incrementable_traversal_tag >::type facade_;
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    T* p;
    explicit traversal(T& x) : p(&x) { }
private:
    friend class sake::iterator::core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
};

template< class T >
class traversal< T, boost::single_pass_traversal_tag >
    : public traversal_facade< T, boost::single_pass_traversal_tag >::type
{
    typedef typename traversal_facade<
        T, boost::single_pass_traversal_tag >::type facade_;
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    T* p;
    explicit traversal(T& x) : p(&x) { }
private:
    friend class sake::iterator::core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
    bool derived_equal(traversal other) const { return p == other.p; }
};

template< class T >
class traversal< T, boost::forward_traversal_tag >
    : public traversal_facade< T, boost::forward_traversal_tag >::type
{
    typedef typename traversal_facade<
        T, boost::forward_traversal_tag >::type facade_;
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    T* p;
    traversal() : p(0) { }
    explicit traversal(T& x) : p(&x) { }
private:
    friend class sake::iterator::core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
    bool derived_equal(traversal other) const { return p == other.p; }
};

template< class T >
class traversal< T, boost::bidirectional_traversal_tag >
    : public traversal_facade< T, boost::bidirectional_traversal_tag >::type
{
    typedef typename traversal_facade<
        T, boost::bidirectional_traversal_tag >::type facade_;
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    T* p;
    traversal() : p(0) { }
    explicit traversal(T& x) : p(&x) { }
private:
    friend class sake::iterator::core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
    void derived_decrement() { }
    bool derived_equal(traversal other) const { return p == other.p; }
};

template< class T >
class traversal< T, boost::random_access_traversal_tag >
    : public traversal_facade< T, boost::random_access_traversal_tag >::type
{
    typedef typename traversal_facade<
        T, boost::random_access_traversal_tag >::type facade_;
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    T* p;
    traversal() : p(0) { }
    explicit traversal(T& x) : p(&x) { }
private:
    friend class sake::iterator::core_access;
    reference derived_dereference() const { return *p; }
    void derived_increment() { }
    void derived_decrement() { }
    void derived_advance_ip(difference_type) { }
    difference_type derived_difference(traversal) const { return 0; }
};

} // namespace

void facade_test(sake::test::environment& env)
{
    {
        typedef traversal< int const, boost::incrementable_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Incrementable< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        ++i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        i++;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
    }
    {
        typedef traversal< int, boost::incrementable_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Incrementable< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        ++i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        i++;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        *i = 1;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env,  x, ==, 1 );
    }
    {
        typedef traversal< int const, boost::single_pass_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::SinglePass< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable< type >));
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
        typedef traversal< int, boost::single_pass_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::SinglePass< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< type >));
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
        typedef traversal< int const, boost::forward_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Forward< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable< type >));
        int x = 0;
        type i(x), j;
        SAKE_TEST_CHECK_RELATION( env, i, !=, j );
    }
    {
        typedef traversal< int, boost::forward_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Forward< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< type >));
        int x = 0;
        type i(x), j;
        SAKE_TEST_CHECK_RELATION( env, i, !=, j );
    }
    {
        typedef traversal< int const, boost::bidirectional_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Bidirectional< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        --i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        i--;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
    }
    {
        typedef traversal< int, boost::bidirectional_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Bidirectional< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< type >));
        int x = 0;
        type i(x);
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        --i;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
        i--;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 0 );
    }
    {
        typedef traversal< int const, boost::random_access_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::RandomAccess< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Readable< type >));
        int x = 0;
        type i(x), j(x);
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK_RELATION( env, i, <=, j );
        SAKE_TEST_CHECK_RELATION( env, i, >=, j );
        SAKE_TEST_CHECK( env, !(i != j) );
        SAKE_TEST_CHECK( env, !(i < j) );
        SAKE_TEST_CHECK( env, !(i > j) );
        i += 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        i -= 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = i + 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = i - 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = 42 + i;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK_RELATION( env, i - j, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, i[42], ==, 0 );
    }
    {
        typedef traversal< int, boost::random_access_traversal_tag > type;
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::RandomAccess< type >));
        BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< type >));
        int x = 0;
        type i(x), j(x);
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK_RELATION( env, i, <=, j );
        SAKE_TEST_CHECK_RELATION( env, i, >=, j );
        SAKE_TEST_CHECK( env, !(i != j) );
        SAKE_TEST_CHECK( env, !(i < j) );
        SAKE_TEST_CHECK( env, !(i > j) );
        i += 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        i -= 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = i + 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = i - 42;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        j = 42 + i;
        SAKE_TEST_CHECK_RELATION( env, i, ==, j );
        SAKE_TEST_CHECK_RELATION( env, i - j, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, i[42], ==, 0 );
        i[42] = 1;
        SAKE_TEST_CHECK_RELATION( env, *i, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env,  x, ==, 1 );
    }
}

} // namespace iterator

} // namespace sake_unit_test
