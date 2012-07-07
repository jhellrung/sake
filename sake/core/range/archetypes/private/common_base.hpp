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

    template< class Introterminal >
    struct iterator_with
        : boost::mpl::apply1< IteratorWith, Introterminal >
    { };
    typedef typename iterator_with<
        sake::null_introterminal_tag >::type iterator;
    typedef iterator const_iterator;

    SAKE_USING_TYPEDEF( typename iterator, reference );

    template< class Introterminal >
    typename iterator_with< Introterminal >::type
    begin(Introterminal) const
    { return *static_cast< typename iterator_with< Introterminal >::type* >(0); }

    template< class Introterminal >
    typename iterator_with< Introterminal >::type
    end(Introterminal) const
    { return *static_cast< typename iterator_with< Introterminal >::type* >(0); }

    template< class Introterminal >
    typename iterator_with< Introterminal >::type
    iter_at(iterator, Introterminal) const
    { return *static_cast< typename iterator_with< Introterminal >::type* >(0); }

    bool empty() const
    { return true; }
    inline friend bool
    range_empty(common_base const & this_)
    { return true; }
};

} // namespace range_private

} // namespace archetypes

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ARCHETYPES_PRIVATE_COMMON_BASE_HPP
