/*******************************************************************************
 * unit_test/test/core/move/movable_test.cpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#include <boost/config.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/move/move.hpp>

#include <sake/test/environment.hpp>
#include <sake/test/test.hpp>

#include "../common/models/basic_movable_copyable.hpp"
#include "../common/models/friendly_movable_copyable.hpp"
#include "../common/models/movable_noncopyable.hpp"
#include "../common/models/movable_uint.hpp"
#include "../common/models/optimal_movable_copyable.hpp"
#include "../common/models/special_mem_fun_stats.hpp"

namespace sake_unit_test
{

void movable_test(sake::test::environment& env)
{
    {
        typedef models::movable_uint type;
        type x(1);
        SAKE_TEST_CHECK_RELATION( env, x.value, !=, 0 );
        type y(x);
        SAKE_TEST_CHECK_RELATION( env, x.value, !=, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.value, !=, 0 );
        type z(sake::move(y));
        SAKE_TEST_CHECK_RELATION( env, y.value, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, z.value, !=, 0 );
        x = z;
        SAKE_TEST_CHECK_RELATION( env, x.value, !=, 0 );
        SAKE_TEST_CHECK_RELATION( env, z.value, !=, 0 );
        y = sake::move(x);
        SAKE_TEST_CHECK_RELATION( env, x.value, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, y.value, !=, 0 );
    }
    {
        typedef models::basic_movable_copyable<> type;
        models::special_mem_fun_stats stats;
        {
            type x(stats);
            type y(x);
            type z(sake::move(y));
            x = z;
            y = sake::move(x);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, ==, 3 );
    }
    {
        typedef models::optimal_movable_copyable<> type;
        models::special_mem_fun_stats stats;
        {
            type x(stats);
            type y(x);
            type z(sake::move(y));
            x = z;
            y = sake::move(x);
            z = sake::construct< type >(stats);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, >=, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, >=, 4 );
    }
    {
        typedef models::friendly_movable_copyable<> type;
        models::special_mem_fun_stats stats;
        {
            type x(stats);
            type y(x);
            type z(sake::move(y));
            x = z;
            y = sake::move(x);
            z = sake::construct< type >(stats);
        }
#ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, >=, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, >=, 4 );
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, >=, 1 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, >=, 4 );
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
    }
    {
        typedef models::movable_noncopyable<> type;
        models::special_mem_fun_stats stats;
        {
            type x(stats);
            type y(sake::move(x));
            type z(sake::construct< type >(stats));
            x = sake::move(z);
            y = sake::construct< type >(stats);
        }
        SAKE_TEST_CHECK_RELATION( env, stats.n_other_constructor, ==, 3 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_constructor, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_constructor, >=, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_copy_assign, ==, 0 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_move_assign, ==, 2 );
        SAKE_TEST_CHECK_RELATION( env, stats.n_destructor, >=, 5 );
    }
}

} // namespace sake_unit_test
