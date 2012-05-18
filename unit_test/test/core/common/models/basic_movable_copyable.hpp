/*******************************************************************************
 * unit_test/test/core/basic_movable_copyable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_BASIC_MOVABLE_COPYABLE_HPP
#define SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_BASIC_MOVABLE_COPYABLE_HPP

#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/assert.hpp>

#include "special_mem_fun_stats.hpp"

namespace sake_unit_test
{

namespace models
{

template< class T = void >
struct basic_movable_copyable;

template<>
struct basic_movable_copyable< void >
{
    SAKE_BASIC_MOVABLE_COPYABLE( basic_movable_copyable )

    explicit basic_movable_copyable(special_mem_fun_stats& stats)
        : m_stats(stats)
    { ++m_stats.n_other_constructor; }

    basic_movable_copyable(basic_movable_copyable const & other)
        : m_stats(other.m_stats)
    { ++m_stats.n_copy_constructor; }

    basic_movable_copyable(this_rvalue_param_type other)
        : m_stats(other.m_stats)
    { ++m_stats.n_move_constructor; }

    ~basic_movable_copyable()
    { ++m_stats.n_destructor; }

    basic_movable_copyable& operator=(basic_movable_copyable const & other)
    {
        SAKE_ASSERT_RELATION( &m_stats, ==, &other.m_stats );
        ++m_stats.n_copy_assign;
        return *this;
    }

    basic_movable_copyable& operator=(this_rvalue_param_type other)
    {
        SAKE_ASSERT_RELATION( &m_stats, ==, &other.m_stats );
        ++m_stats.n_move_assign;
        return *this;
    }

private:
    special_mem_fun_stats& m_stats;
};

template< class T >
struct basic_movable_copyable
    : basic_movable_copyable<>
{
    T value;

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename basic_movable_copyable,
        (( basic_movable_copyable<> )) (( T )( value ))
    )

    explicit basic_movable_copyable(special_mem_fun_stats& stats)
        : basic_movable_copyable<>(stats)
    { }

    template< class Signature >
    basic_movable_copyable(special_mem_fun_stats& stats, T value_)
        : basic_movable_copyable<>(stats),
          value(sake::move(value_))
    { }
};

} // namespace models

} // namespace sake_unit_test

#endif // #ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_BASIC_MOVABLE_COPYABLE_HPP
