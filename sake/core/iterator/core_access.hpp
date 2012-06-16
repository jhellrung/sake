/*******************************************************************************
 * sake/core/iterator/core_access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::core_access
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CORE_ACCESS_HPP
#define SAKE_CORE_ITERATOR_CORE_ACCESS_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/common_difference_type.hpp>
#include <sake/core/iterator/private/facade/compare_enable.hpp>
#include <sake/core/iterator/private/facade/difference_enable.hpp>
#include <sake/core/iterator/private/facade/equal_enable.hpp>
#include <sake/core/math/sign_t_fwd.hpp>

namespace sake
{

namespace iterator
{

class core_access
{

#define declare_friend( x, y ) \
template< class D0, class P0, class D1, class P1 > \
    friend typename sake::iterator::facade_adl::private_:: \
        x ## _enabler< D0, P0, D1, P1 >::type \
    sake::iterator::facade_adl::y( \
        sake::iterator::facade_adl::facade< D0, P0 > const & i0, \
        sake::iterator::facade_adl::facade< D1, P1 > const & i1);
    declare_friend( equal, operator== )
    declare_friend( compare, operator< )
    declare_friend( cmp, cmp )
    declare_friend( difference, operator- )
#undef declare_friend

    template< class, class >
    friend class sake::iterator::facade_adl::facade;
    template< class, class >
    friend class sake::iterator::facade_adl::private_::common_base;
#define declare_friend( x ) \
    template< class, class, int > \
    friend class sake::iterator::facade_adl::private_::x ## _base;
    declare_friend( traversal )
    declare_friend( begin_introversal )
    declare_friend( end_introversal )
    declare_friend( chained )
#undef declare_friend

    // Dereferenceable
    template< class Derived >
    static typename Derived::reference
    dereference(Derived const & this_)
    { return this_.derived_dereference(); }

    // Incrementable
    template< class Derived >
    static void
    increment(Derived& this_)
    { this_.derived_increment(); }

    // SinglePass
    template< class I, class J >
    static typename boost::enable_if_c<
        boost_ext::is_convertible<J,I>::value, bool >::type
    equal(I const & i, J const & j)
    { return i.derived_equal(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value, bool >::type
    equal(I const & i, J const & j)
    { return j.derived_equal(i); }

    // Bidirectional
    template< class Derived >
    static void
    decrement(Derived& this_)
    { this_.derived_decrement(); }

    // RandomAccess
    template< class Derived >
    static void
    advance_ip(Derived& this_, typename Derived::difference_type const n)
    { this_.derived_advance_ip(n); }
    template< class Derived >
    static Derived
    advance(Derived const & this_, typename Derived::difference_type const n)
    { return this_.derived_advance(n); }

    template< class I0, class I1 >
    static typename boost::lazy_enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value,
        sake::iterator::private_::common_difference_type< I0, I1 > >::type
    difference(I0 const & i0, I1 const & i1)
    { return i0.derived_difference(i1); }
    template< class I0, class I1 >
    static typename boost::lazy_disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value,
        sake::iterator::private_::common_difference_type< I1, I0 > >::type
    difference(I0 const & i0, I1 const & i1)
    { return -i1.derived_difference(i0); }

    template< class I0, class I1 >
    static typename boost::enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, bool >::type
    less(I0 const & i0, I1 const & i1)
    { return i0.derived_less(i1); }
    template< class I0, class I1 >
    static typename boost::disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, bool >::type
    less(I0 const & i0, I1 const & i1)
    { return !i1.derived_less_equal(i0); }

    template< class I0, class I1 >
    static typename boost::enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, sake::sign_t >::type
    cmp(I0 const & i0, I1 const & i1)
    { return i0.derived_cmp(i1); }
    template< class I0, class I1 >
    static typename boost::disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, sake::sign_t >::type
    cmp(I0 const & i0, I1 const & i1)
    { return -i1.derived_cmp(i0); }

    // Introversal
    template< class Derived, class Introversal >
    struct relax
    {
        typedef typename Derived::template
            derived_relax< Introversal >::type type;
    };
    template< class Derived, class Other >
    static void
    at_ip(Derived& this_, Other const & other)
    { return this_.derived_at_ip(other); }
    template< class Derived, class Other, class Introversal >
    static typename Derived::template relax< Introversal >::type
    at(Derived const & this_, Other const & other, Introversal)
    { return this_.derived_at(other, Introversal()); }

    // BeginDetect
    template< class Derived >
    static bool
    equal_begin(Derived const & this_)
    { return this_.derived_equal_begin(); }

    // BeginAccess
    template< class Derived >
    static void
    at_begin_ip(Derived& this_)
    { return this_.derived_at_begin_ip(); }
    template< class Derived, class Introversal >
    static typename Derived::template relax< Introversal >::type
    at_begin(Derived const & this_, Introversal)
    { return this_.derived_at_begin(Introversal()); }

    // EndDetect
    template< class Derived >
    static bool
    equal_end(Derived const & this_)
    { return this_.derived_equal_end(); }

    // EndAccess
    template< class Derived >
    static void
    at_end_ip(Derived& this_)
    { return this_.derived_at_end_ip(); }
    template< class Derived, class Introversal >
    static typename Derived::template relax< Introversal >::type
    at_end(Derived const & this_, Introversal)
    { return this_.derived_at_end(Introversal()); }

    // RandomAccess + BeginAccess
    template< class Derived >
    static typename Derived::difference_type
    difference_begin(Derived const & this_)
    { return this_.derived_difference_begin(); }

    // RandomAccess + EndAccess
    template< class Derived >
    static typename Derived::difference_type
    difference_end(Derived const & this_)
    { return this_.derived_difference_end(); }
};

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CORE_ACCESS_HPP
