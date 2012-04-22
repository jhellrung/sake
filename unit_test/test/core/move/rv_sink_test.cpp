/*******************************************************************************
 * unit_test/test/core/move/rv_sink_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/move/rv_sink.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace
{

template< class T = void >
struct rvalue_check
{
    typedef int result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    int operator()(U&&) const
    { return sake::boost_ext::is_rvalue_reference< U&& >::value; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<T> rv_sink_traits_;
    typedef typename rv_sink_traits_::template
        default_< rvalue_check > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class U >
    int operator()(U& x) const
    { return sake::boost_ext::is_rvalue_reference< U& >::value; }

    // T rvalues
    int operator()(typename rv_sink_traits_::primary_type) const
    { return 1; }

    // movable implicit rvalues
    int operator()(rv_sink_default_type x) const
    { return 1 + x(); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::template enable_cref< U, int >::type
    operator()(U const &) const
    { return 0; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

template<>
struct rvalue_check< void >
{
    typedef int result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    int operator()(U&&) const
    { return sake::boost_ext::is_rvalue_reference< U&& >::value; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    typedef rv_sink_traits_::default_< rvalue_check > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class U >
    int operator()(U& x) const
    { return sake::boost_ext::is_rvalue_reference< U& >::value; }

    // movable implicit rvalues
    int operator()(rv_sink_default_type x) const
    { return 1 + x(); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::enable_cref< U, int >::type
    operator()(U const &) const
    { return 0; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace

void rv_sink_test(sake::test::environment& env)
{
    typedef models::basic_movable_copyable<> type;
    typedef models::basic_movable_copyable< int > int_type;
    models::special_mem_fun_stats stats;
    int i = 0;
    type x(stats);
    type const cx(stats);

    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(i) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(0) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(x) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(cx) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check<>()(sake::move(x)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(sake::move(cx)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check<>()(sake::move(cx)) );
#ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check<>()(sake::construct< type >(stats)) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check<>()(sake::construct< int_type >(stats)) );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 2, ==, rvalue_check<>()(sake::construct< type >(stats)) );
    SAKE_TEST_CHECK_RELATION( env, 2, ==, rvalue_check<>()(sake::construct< int_type >(stats)) );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(i) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(0) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(x) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(cx) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check< type >()(sake::move(x)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(sake::move(cx)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, rvalue_check< type >()(sake::move(cx)) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check< type >()(sake::construct< type >(stats)) );
#ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 1, ==, rvalue_check< type >()(sake::construct< int_type >(stats)) );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 2, ==, rvalue_check< type >()(sake::construct< int_type >(stats)) );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

}

} // namespace sake_unit_test
