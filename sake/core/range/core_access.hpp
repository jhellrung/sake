/*******************************************************************************
 * sake/core/range/core_access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::core_access
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CORE_ACCESS_HPP
#define SAKE_CORE_RANGE_CORE_ACCESS_HPP

#include <sake/core/range/facade_fwd.hpp>

namespace sake
{

namespace range
{

class core_access
{
    template< class, class >
    friend class sake::range::facade_adl::facade;
    template< class, class, int >
    friend class sake::range::facade_adl::private_::size_base;

    template< class Derived, class Introversal >
    struct iterator_with
    {
        typedef typename Derived::template
            derived_iterator_with_of< Derived, Introversal >::type type;
    };

    template< class Derived >
    struct reference
    { typedef typename Derived::reference type; };
    template< class Derived >
    struct reference< Derived const >
    { typedef typename Derived::const_reference type; };

    template< class Derived, class T, class Introversal >
    static typename iterator_with< Derived, Introversal >::type
    iter_at(Derived& this_, T const & x, Introversal)
    { return Derived::derived_iter_at(this_, x, Introversal()); }

    template< class Derived, class T >
    static typename reference< Derived >::type
    at(Derived& this_, T const x)
    { return Derived::derived_at(this_, x); }

    template< class Derived >
    static bool
    empty(Derived const & this_)
    { return this_.derived_empty(); }

    template< class Derived >
    static typename Derived::difference_type
    distance(Derived const & this_)
    { return this_.derived_distance(); }

    template< class Derived >
    static typename Derived::size_type
    size(Derived const & this_)
    { return this_.derived_size(); }
};

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CORE_ACCESS_HPP
