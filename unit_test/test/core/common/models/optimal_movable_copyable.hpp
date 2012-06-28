/*******************************************************************************
 * unit_test/test/core/optimal_movable_copyable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_OPTIMAL_MOVABLE_COPYABLE_HPP
#define SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_OPTIMAL_MOVABLE_COPYABLE_HPP

#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>

#include "basic_movable_copyable.hpp"
#include "special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace models
{

template< class T = void >
struct optimal_movable_copyable
    : basic_movable_copyable<T>
{
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename optimal_movable_copyable,
        (( basic_movable_copyable<T> ))
    )

    explicit optimal_movable_copyable(special_mem_fun_stats& stats)
        : basic_movable_copyable<T>(stats)
    { }

    optimal_movable_copyable(special_mem_fun_stats& stats, T value_)
        : basic_movable_copyable<T>(stats, sake::move(value_))
    { }
};

template<>
struct optimal_movable_copyable< void >
    : basic_movable_copyable<>
{
    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        optimal_movable_copyable,
        (( basic_movable_copyable<> ))
    )

    explicit optimal_movable_copyable(special_mem_fun_stats& stats)
        : basic_movable_copyable<>(stats)
    { }
};

} // namespace models

} // namespace sake_unit_test

#endif // #ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_BASIC_MOVABLE_COPYABLE_HPP
