/*******************************************************************************
 * sake/core/iterator/iterator_core_access.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator_core_access
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP
#define SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/common_difference_type.hpp>
#include <sake/core/iterator/private/facade/operator_equality_enable.hpp>
#include <sake/core/iterator/private/facade/operator_minus_enable.hpp>
#include <sake/core/iterator/private/facade/operator_relational_enable.hpp>
#include <sake/core/math/sign_t_fwd.hpp>

namespace sake
{

class iterator_core_access
{

#define declare_friend( x, y ) \
template< class D0, class P0, class D1, class P1 > \
    friend typename sake::iterator_facade_adl::private_:: \
        x ## _enabler< D0, P0, D1, P1 >::type \
    sake::iterator_facade_adl::y( \
        sake::iterator_facade_adl::iterator_facade< D0, P0 > const & i0, \
        sake::iterator_facade_adl::iterator_facade< D1, P1 > const & i1);
    declare_friend( operator_equality, operator== )
    declare_friend( operator_relational, operator< )
    declare_friend( cmp, cmp )
    declare_friend( operator_minus, operator- )
#undef declare_friend

#define declare_friend( x ) \
    template< class D, class P > \
    friend typename sake::iterator_facade_adl::private_:: \
        operator_minus_ ## x ## _enabler<D,P>::type \
    sake::iterator_facade_adl::operator-( \
        sake::iterator_facade_adl::iterator_facade<D,P> const & i, \
        sake::x ## _tag);
    declare_friend( begin )
    declare_friend( end )
#undef declare_friend

    template< class, class >
    friend class sake::iterator_facade_adl::iterator_facade;
#define declare_friend( x ) \
    template< class, class, int > \
    friend class sake::iterator_facade_adl::private_::x ## _base;
    declare_friend( traversal )
    declare_friend( begin_introversal )
    declare_friend( end_introversal )
    declare_friend( common )
#undef declare_friend

    // Dereferenceable
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
        boost_ext::is_convertible<J,I>::value, bool >::type
    operator_equal(I const & i, J const & j)
    { return i.operator_equal_impl(j); }
    template< class I, class J >
    static typename boost::disable_if_c<
        boost_ext::is_convertible<J,I>::value, bool >::type
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

    template< class I0, class I1 >
    static typename boost::enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, bool >::type
    operator_less(I0 const & i0, I1 const & i1)
    { return i0.operator_less_impl(i1); }
    template< class I0, class I1 >
    static typename boost::disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, bool >::type
    operator_less(I0 const & i0, I1 const & i1)
    { return !i1.operator_less_equal_impl(i0); }

    template< class I0, class I1 >
    static typename boost::enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, sake::sign_t >::type
    cmp(I0 const & i0, I1 const & i1)
    { return i0.cmp_impl(i1); }
    template< class I0, class I1 >
    static typename boost::disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value, sake::sign_t >::type
    cmp(I0 const & i0, I1 const & i1)
    { return -i1.cmp_impl(i0); }

    template< class I0, class I1 >
    static typename boost::lazy_enable_if_c<
        boost_ext::is_convertible< I1, I0 >::value,
        sake::iterator_private::common_difference_type< I0, I1 > >::type
    operator_minus(I0 const & i0, I1 const & i1)
    { return i0.operator_minus_impl(i1); }
    template< class I0, class I1 >
    static typename boost::lazy_disable_if_c<
        boost_ext::is_convertible< I1, I0 >::value,
        sake::iterator_private::common_difference_type< I1, I0 > >::type
    operator_minus(I0 const & i0, I1 const & i1)
    { return -i1.operator_minus_impl(i0); }

    // BeginDetect
    template< class Derived >
    static bool
    at_begin(Derived const & this_)
    { return this_.at_begin_impl(); }

    // BeginAccess
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
    static Derived
    end(Derived const & this_)
    { return this_.end_impl(); }

    // RandomAccess + BeginAccess
    template< class Derived >
    static typename Derived::difference_type
    operator_minus_begin(Derived const & this_)
    { return this_.operator_minus_begin_impl(); }

    // RandomAccess + EndAccess
    template< class Derived >
    static typename Derived::difference_type
    operator_minus_end(Derived const & this_)
    { return this_.operator_minus_end_impl(); }
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ITERATOR_CORE_ACCESS_HPP
