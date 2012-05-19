/*******************************************************************************
 * unit_test/test/core/move/rv_sink_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>

#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

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
struct helper
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
        default_< helper > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class U >
    int operator()(U&) const
    { return sake::boost_ext::is_rvalue_reference< U& >::value; }

    // T rvalues
    int operator()(typename rv_sink_traits_::primary_type) const
    { return 1; }

    // movable implicit rvalues
    int operator()(rv_sink_default_type x) const
    { return 1 + x(); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::template cref_enabler< U, int >::type
    operator()(U const &) const
    { return 0; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

template<>
struct helper< void >
{
    typedef int result_type;

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    int operator()(U&&) const
    { return sake::boost_ext::is_rvalue_reference< U&& >::value; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits<> rv_sink_traits_;
    typedef rv_sink_traits_::default_< helper > rv_sink_default_type;
public:

    // lvalues + movable explicit rvalues
    template< class U >
    int operator()(U&) const
    { return sake::boost_ext::is_rvalue_reference< U& >::value; }

    // movable implicit rvalues
    int operator()(rv_sink_default_type x) const
    { return 1 + x(); }

    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::cref_enabler< U, int >::type
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

    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(i) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(x) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(cx) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(sake::move(i)) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper<>()(sake::move(x)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(sake::move(cx)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()(0) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper<>()((int(0))) );
#ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper<>()((type(stats))) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper<>()((int_type(stats))) );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 2, ==, helper<>()((type(stats))) );
    SAKE_TEST_CHECK_RELATION( env, 2, ==, helper<>()((int_type(stats))) );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(i) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(x) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(cx) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(sake::move(i)) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper< type >()(sake::move(x)) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(sake::move(cx)) );
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper< type >()((type(stats))) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()(0) );
    SAKE_TEST_CHECK_RELATION( env, 0, ==, helper< type >()((int(0))) );
#ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 1, ==, helper< type >()((int_type(stats))) );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    SAKE_TEST_CHECK_RELATION( env, 2, ==, helper< type >()((int_type(stats))) );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

}

} // namespace sake_unit_test
