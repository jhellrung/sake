/*******************************************************************************
 * sake/core/iterator/iterator_core_access.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator_core_access
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/private/facade/common_difference_type.hpp>
#include <sake/core/iterator/private/facade/function_prototype.hpp>
#include <sake/core/math/sign_t_fwd.hpp>

namespace sake
{

template< class, class, class, class, class, class, class > struct iterator_facade;

class iterator_core_access
{
    template< class, class, class, class, class, class, class > friend struct iterator_facade;

    SAKE_ITERATOR_FACADE_function_prototype( friend, boost::mpl::always< bool >, operator== );
    SAKE_ITERATOR_FACADE_function_prototype( friend, boost::mpl::always< bool >, operator<  );
    SAKE_ITERATOR_FACADE_function_prototype( friend, boost::mpl::always< bool >, operator<= );
    SAKE_ITERATOR_FACADE_function_prototype( friend, boost::mpl::always< sake::sign_t >, compare );
    SAKE_ITERATOR_FACADE_function_prototype(
        friend, boost::mpl::quote2< iterator_facade_private::common_difference_type >, operator- );

    // Readable/Writable
    template< class Derived >
    static typename Derived::reference
    dereference(Derived const & this_)
    { return this_.dereference_impl(); }

    // Incrementable
    template< class Derived >
    static void
    increment(Derived& this_)
    { this_.increment_impl(); }

    // SinglePass
    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    equal(I const & i, J const & j)
    { return i.equal_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    equal(I const & i, J const & j)
    { return j.equal_impl(i); }

    // Bidirectional
    template< class Derived >
    static void
    decrement(Derived& this_)
    { this_.decrement_impl(); }

    // RandomAccess
    template< class Derived >
    static void
    advance(Derived& this_, typename Derived::difference_type const & n)
    { this_.advance_impl(n); }

    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        sake::sign_t
    >::type
    compare(I const & i, J const & j)
    { return i.compare_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        sake::sign_t
    >::type
    compare(I const & i, J const & j)
    { return -j.compare_impl(i); }

    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    less(I const & i, J const & j)
    { return i.less_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    less(I const & i, J const & j)
    { return !j.less_equal_impl(i); }

    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    less_equal(I const & i, J const & j)
    { return i.less_equal_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    less_equal(I const & i, J const & j)
    { return !j.less_impl(i); }

    template< class I, class J >
    static typename boost::lazy_enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        iterator_facade_private::common_difference_type<I,J>
    >::type
    difference(I const & i, J const & j)
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            typename I::iterator_traversal,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            typename J::iterator_traversal,
            boost::random_access_traversal_tag
        >::value));
        return i.difference_impl(j);
    }
    template< class I, class J >
    static typename boost::lazy_disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        iterator_facade_private::common_difference_type<J,I>
    >::type
    difference(I const & i, J const & j)
    {
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            typename I::iterator_traversal,
            boost::random_access_traversal_tag
        >::value));
        BOOST_STATIC_ASSERT((boost_ext::is_convertible<
            typename J::iterator_traversal,
            boost::random_access_traversal_tag
        >::value));
        return -j.difference_impl(i);
    }

    // BeginDetect
    template< class Derived >
    static bool
    at_begin(Derived const & this_)
    { return this_.at_begin_impl(); }

    // BeginAccess
    template< class Derived >
    static void
    to_begin(Derived& this_)
    { this_.to_begin_impl(); }
    template< class Derived >
    static Derived
    begin(Derived const & this_)
    { return this_.begin_impl(); }

    // EndDetect
    template< class Derived >
    static bool
    at_end(Derived const & this_)
    { return this_.at_end_impl(); }

    // EndAccess
    template< class Derived >
    static void
    to_end(Derived& this_)
    { this_.to_end_impl(); }
    template< class Derived >
    static Derived
    end(Derived const & this_)
    { return this_.end_impl(); }

    // Bidirectional + BeginDetect
    template< class Derived >
    static typename Derived::difference_type
    begin_distance(Derived const & this_)
    { return this_.begin_distance_impl(); }

    // Forward + EndDetect
    template< class Derived >
    static typename Derived::difference_type
    end_distance(Derived const & this_)
    { return this_.end_distance_impl(); }

    // Bidirectional + BeginDetect + EndDetect
    template< class Derived >
    static typename Derived::difference_type
    begin_end_distance(Derived const & this_)
    { return this_.begin_end_distance_impl(); }
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP
