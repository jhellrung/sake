/*******************************************************************************
 * sake/core/memory/alloc/expand_array_op.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_EXPAND_ARRAY_OP_HPP
#define SAKE_CORE_MEMORY_ALLOC_EXPAND_ARRAY_OP_HPP

#include <cstddef>

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/math/zero.hpp>
#include <sake/core/memory/alloc/expand_array_tag.hpp>

namespace sake
{

namespace alloc
{

template< class Size = std::size_t >
struct expand_array_op
{
    typedef Size size_type;

    alloc::expand_array_tag::type tag;
    size_type limit_count;
    size_type preferred_count;

    expand_array_op(
        alloc::expand_array_tag::type const tag_,
        size_type const limit_count_,
        size_type const preferred_count = sake::zero.as< size_type >())
        : tag(tag_),
          limit_count(limit_count_),
          preferred_count(preferred_count_)
    { }

    template< class Size_ >
    expand_array_op(expand_array_op< Size_ > const & other,
        typename boost::enable_if_c<
            boost_ext::is_convertible< Size_, Size >::value >::type* = 0)
        : tag(other.tag),
          limit_count(other.limit_count),
          preferred_count(other.preferred_count)
    { }
};

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_EXPAND_ARRAY_OP_HPP
