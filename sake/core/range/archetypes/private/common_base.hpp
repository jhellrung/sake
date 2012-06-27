/*******************************************************************************
 * sake/core/range/archetypes/common_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_COMMON_BASE_HPP
#define SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_COMMON_BASE_HPP

#include <boost/mpl/apply.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/utility/noncopyable.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace archetypes
{

namespace range_private
{

template< class IteratorWith >
struct common_base
{
    SAKE_NONCOPYABLE( common_base )

    template< class Introversal >
    struct iterator_with
        : boost::mpl::apply1< IteratorWith, Introversal >
    { };
    typedef typename iterator_with<
        sake::null_introversal_tag >::type iterator;
    typedef iterator const_iterator;

    SAKE_USING_TYPEDEF( typename iterator, reference );

    template< class Introversal >
    typename iterator_with< Introversal >::type
    begin(Introversal) const
    { return *static_cast< typename iterator_with< Introversal >::type* >(0); }

    template< class Introversal >
    typename iterator_with< Introversal >::type
    end(Introversal) const
    { return *static_cast< typename iterator_with< Introversal >::type* >(0); }

    template< class Introversal >
    typename iterator_with< Introversal >::type
    iter_at(iterator, Introversal) const
    { return *static_cast< typename iterator_with< Introversal >::type* >(0); }

    bool empty() const
    { return true; }
    inline friend
    bool range_empty(common_base const & this_)
    { return this_.empty(); }
};

} // namespace range_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_COMMON_BASE_HPP
