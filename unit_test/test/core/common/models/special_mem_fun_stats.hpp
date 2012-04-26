/*******************************************************************************
 * unit_test/test/core/common/models/special_mem_fun_stats.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_SPECIAL_MEM_FUN_STATS_HPP
#define SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_SPECIAL_MEM_FUN_STATS_HPP

namespace sake_unit_test
{

namespace models
{

struct special_mem_fun_stats
{
    unsigned int n_other_constructor;
    unsigned int n_copy_constructor;
    unsigned int n_move_constructor;
    unsigned int n_copy_assign;
    unsigned int n_move_assign;
    unsigned int n_destructor;

    special_mem_fun_stats()
        : n_other_constructor(0),
          n_copy_constructor(0),
          n_move_constructor(0),
          n_copy_assign(0),
          n_move_assign(0),
          n_destructor(0)
    { }

    void reset()
    {
        n_other_constructor = 0;
        n_copy_constructor = 0;
        n_move_constructor = 0;
        n_copy_assign = 0;
        n_move_assign = 0;
        n_destructor = 0;
    };
};

} // namespace models

} // namespace sake_unit_test

#endif // #ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_SPECIAL_MEM_FUN_STATS_HPP
