/*******************************************************************************
 * unit_test/test/core/common/models/movable_uint.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_UINT_HPP
#define SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_UINT_HPP

#include <sake/core/move/movable.hpp>
#include <sake/core/utility/assert.hpp>

namespace sake_unit_test
{

namespace models
{

struct movable_uint
{
    unsigned int value;

    SAKE_BASIC_MOVABLE_COPYABLE( movable_uint )

    movable_uint(unsigned int const value_ = 0)
        : value(value_)
    { }

    movable_uint(movable_uint const & other)
        : value(other.value + 1)
    { }

    movable_uint(this_rvalue_param_type other)
        : value(other.value)
    { other.value = 0; }

    movable_uint& operator=(movable_uint const & other)
    {
        value = other.value + 1;
        return *this;
    }

    movable_uint& operator=(this_rvalue_param_type other)
    {
        SAKE_ASSERT_RELATION( this, !=, &other );
        value = other.value;
        other.value = 0;
        return *this;
    }
};

} // namespace models

} // namespace sake_unit_test

#endif // #ifndef SAKE_UNIT_TEST_TEST_CORE_COMMON_MODELS_MOVABLE_UINT_HPP
