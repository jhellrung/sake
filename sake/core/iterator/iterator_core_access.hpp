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
    SAKE_ITERATOR_FACADE_function_prototype( friend, boost::mpl::always< sake::sign_t >, compare );
    SAKE_ITERATOR_FACADE_function_prototype(
        friend,
        boost::mpl::quote2< iterator_facade_private::common_difference_type >,
        operator-
    );

    // Readable/Writable
    template< class Derived >
    static typename Derived::reference
    operator_star(Derived const & this_)
    { return this_.operator_star_impl(); }

    // Incrementable
    template< class Derived >
    static void
    operator_pre_increment(Derived& this_)
    { this_.operator_pre_increment_impl(); }

    // SinglePass
    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    operator_equal(I const & i, J const & j)
    { return i.operator_equal_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    operator_equal(I const & i, J const & j)
    { return j.operator_equal_impl(i); }

    // Bidirectional
    template< class Derived >
    static void
    operator_pre_decrement(Derived& this_)
    { this_.operator_pre_decrement_impl(); }

    // RandomAccess
    template< class Derived >
    static void
    operator_plus_equal(Derived& this_, typename Derived::difference_type const n)
    { this_.operator_plus_equal_impl(n); }
    template< class Derived >
    static Derived
    operator_plus(Derived const & this_, typename Derived::difference_type const n)
    { return this_.operator_plus_impl(n); }

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
    { return i.operator_less_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        bool
    >::type
    less(I const & i, J const & j)
    { return !j.operator_less_equal_impl(i); }

    template< class I, class J >
    static typename boost::lazy_enable_if_c<
        boost_ext::is_convertible<J,I>::value,
        iterator_facade_private::common_difference_type<I,J>
    >::type
    operator_minus(I const & i, J const & j)
    { return i.operator_minus_impl(j); }
    template< class I, class J >
    static typename boost::lazy_disable_if_c<
        boost_ext::is_convertible<J,I>::value,
        iterator_facade_private::common_difference_type<J,I>
    >::type
    operator_minus(I const & i, J const & j)
    { return -j.operator_minus_impl(i); }

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

    // RandomAccess + BeginAccess
    template< class Derived >
    static typename Derived::difference_type
    distance_from_begin(Derived const & this_)
    { return this_.distance_from_begin_impl(); }

    // RandomAccess + EndAccess
    template< class Derived >
    static typename Derived::difference_type
    distance_to_end(Derived const & this_)
    { return this_.distance_to_end_impl(); }

    // RandomAccess + BeginAccess + EndAccess
    template< class Derived >
    static typename Derived::difference_type
    distance_between_begin_end(Derived const & this_)
    { return this_.distance_between_begin_end_impl(); }
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP
