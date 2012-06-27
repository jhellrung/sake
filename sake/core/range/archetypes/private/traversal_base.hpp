/*******************************************************************************
 * sake/core/range/archetypes/private/traversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP

#include <cstddef>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/archetypes/private/common_base.hpp>
#include <sake/core/range/archetypes/private/reference_dispatch.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace archetypes
{

namespace range_private
{

template< class Traversal, class IteratorWith >
struct traversal_base;

template< class IteratorWith >
struct traversal_base< boost::single_pass_traversal_tag, IteratorWith >
    : sake::archetypes::range_private::common_base< IteratorWith >
{
private:
    typedef sake::archetypes::range_private::
        common_base< IteratorWith > common_base_;
public:
    SAKE_USING_TYPEDEF( typename common_base_, reference );

    reference front() const
    { return range_private::reference_dispatch< reference >::apply(); }
};

template< class IteratorWith >
struct traversal_base< boost::forward_traversal_tag, IteratorWith >
    : sake::archetypes::range_private::traversal_base<
          boost::single_pass_traversal_tag, IteratorWith >
{
private:
    typedef sake::archetypes::range_private::traversal_base<
        boost::single_pass_traversal_tag, IteratorWith > traversal_base_;
public:
    typedef std::ptrdiff_t difference_type;

    difference_type distance() const
    { return 0; }
    inline friend
    difference_type range_distance(traversal_base const & this_)
    { return this_.distance(); }
};

template< class IteratorWith >
struct traversal_base< boost::bidirectional_traversal_tag, IteratorWith >
    : sake::archetypes::range_private::traversal_base<
          boost::forward_traversal_tag, IteratorWith >
{
private:
    typedef sake::archetypes::range_private::traversal_base<
        boost::forward_traversal_tag, IteratorWith > traversal_base_;
public:
    SAKE_USING_TYPEDEF( typename traversal_base_, reference );

    reference back() const
    { return range_private::reference_dispatch< reference >::apply(); }
};

template< class IteratorWith >
struct traversal_base< boost::random_access_traversal_tag, IteratorWith >
    : sake::archetypes::range_private::traversal_base<
          boost::bidirectional_traversal_tag, IteratorWith >
{
private:
    typedef sake::archetypes::range_private::traversal_base<
        boost::bidirectional_traversal_tag, IteratorWith > traversal_base_;
public:
    SAKE_USING_TYPEDEF( typename traversal_base_, reference );
    typedef std::size_t size_type;

    reference operator[](size_type const) const
    { return range_private::reference_dispatch< reference >::apply(); }

    size_type size() const
    { return 0; }
    inline friend
    size_type range_calculate_size(traversal_base const & this_)
    { return this_.size(); }
};

} // namespace range_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_TRAVERSAL_BASE_HPP
