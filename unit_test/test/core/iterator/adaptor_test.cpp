/*******************************************************************************
 * unit_test/test/core/iterator/adaptor_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/concept/assert.hpp>
#include <boost/mpl/map/map10.hpp>

#include <sake/core/iterator/adaptor.hpp>
#include <sake/core/iterator/concepts/Bidirectional.hpp>
#include <sake/core/iterator/concepts/WritableLvalue.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/keyword.hpp>
#include <sake/core/utility/nullptr.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

namespace sake_unit_test
{

namespace iterator
{

namespace
{

SAKE_USING_NULLPTR;

struct X
{
    X* p_prior;
    X* p_next;

    X() : p_prior(0), p_next(0) { }

    class iterator
        : public sake::iterator::adaptor<
              iterator, X*,
              boost::mpl::map1<
                  sake::iterator::keyword::bidirectional_traversal
              >
          >
    {
        typedef sake::iterator::adaptor<
            iterator, X*,
            boost::mpl::map1<
                sake::iterator::keyword::bidirectional_traversal
            >
        > adaptor_;
    public:
        iterator()
            : adaptor_(nullptr)
        { }
        explicit iterator(X* p)
            : adaptor_(p)
        { }
    private:
        using adaptor_::protected_base;
        friend class sake::iterator::core_access;
        void derived_increment()
        { protected_base() = protected_base()->p_next; }
        void derived_decrement()
        { protected_base() = protected_base()->p_prior; }
    };
};

BOOST_CONCEPT_ASSERT((sake::iterator::concepts::Bidirectional< X::iterator >));
BOOST_CONCEPT_ASSERT((sake::iterator::concepts::WritableLvalue< X::iterator >));

} // namespace

void adaptor_test(sake::test::environment& env)
{
    X x0;
    X x1;
    X x2;
    x0.p_next = &x1; x1.p_prior = &x0;
    x1.p_next = &x2; x2.p_prior = &x1;
    X::iterator i(&x0);
    SAKE_TEST_CHECK_RELATION( env, &*i, ==, &x0 );
    ++i;
    SAKE_TEST_CHECK_RELATION( env, &*i, ==, &x1 );
    ++i;
    SAKE_TEST_CHECK_RELATION( env, &*i, ==, &x2 );
    --i;
    SAKE_TEST_CHECK_RELATION( env, &*i, ==, &x1 );
    --i;
    SAKE_TEST_CHECK_RELATION( env, &*i, ==, &x0 );
}

} // namespace iterator

} // namespace sake_unit_test
