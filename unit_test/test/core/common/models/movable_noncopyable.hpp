/*******************************************************************************
 * unit_test/test/core/movable_noncopyable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_NONCOPYABLE_HPP
#define SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_NONCOPYABLE_HPP

#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>

#include "basic_movable_copyable.hpp"
#include "special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace models
{

template< class T = void >
struct movable_noncopyable
    : basic_movable_copyable<T>
{
    SAKE_MOVABLE_NONCOPYABLE( movable_noncopyable )
    SAKE_MEMBERWISE_MEM_FUN(
        typename movable_noncopyable,
        ( move_constructor ) ( move_assign ),
        (( basic_movable_copyable<T> ))
    )

    explicit movable_noncopyable(special_mem_fun_stats& stats)
        : basic_movable_copyable<T>(stats)
    { }

    template< class Signature >
    movable_noncopyable(special_mem_fun_stats& stats, T value_)
        : basic_movable_copyable<T>(stats, sake::move(value_))
    { }
};

template<>
struct movable_noncopyable< void >
    : basic_movable_copyable<>
{
    SAKE_MOVABLE_NONCOPYABLE( movable_noncopyable )
    SAKE_MEMBERWISE_MEM_FUN(
        movable_noncopyable,
        ( move_constructor ) ( move_assign ),
        (( basic_movable_copyable<> ))
    )

    explicit movable_noncopyable(special_mem_fun_stats& stats)
        : basic_movable_copyable<>(stats)
    { }
};

} // namespace models

} // namespace sake_unit_test

#endif // #ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_NONCOPYABLE_HPP
