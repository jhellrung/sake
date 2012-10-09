/*******************************************************************************
 * sake/core/range/facade_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class range::facade< Derived, Params >
 *
 * Keyword parameters accepted in Params:
 * - range::keyword::iterator
 * - range::keyword::const_iterator
 * - range::keyword::back_enable
 * - range::keyword::distance_enable
 * - range::keyword::size_enable
 * - range::keyword::chained_base
 *
 * Implementations to be defined in Derived:
 * - struct derived_iterator_with_of< This, Introterminal >
 *     { typedef ... type; };
 * - static derived_iter_at(This& this_, T x, Introterminal)
 *     -> iterator_with_of< This, Introterminal >::type
 * - struct derived_subrange_with_of< This, Begin, End >
 *     { typedef ... type; };
 * - static derived_sub(This& this_, Begin b, End e)
 *     -> subrange_of< This, Begin, End >::type
 * - static derived_at(This& this_, T x)
 *     -> reference_of< This >::type [defaulted]
 * - derived_empty() const -> bool [defaulted]
 * - derived_distance() const -> difference_type
 *     [if distance_enable::value == true; defaulted]
 * - derived_size() const -> size_type
 *     [if size_enable::value == true; defaulted]
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FACADE_HPP
#define SAKE_CORE_RANGE_FACADE_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/math/prior.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/debug.hpp>
#include <sake/core/range/default_impl/distance.hpp>
#include <sake/core/range/default_impl/empty.hpp>
#include <sake/core/range/default_impl/size.hpp>
#include <sake/core/range/facade_fwd.hpp>
#include <sake/core/range/private/facade/at_enable.hpp>
#include <sake/core/range/private/facade/explicit_constructor_enable.hpp>
#include <sake/core/range/private/facade/front_back_base.hpp>
#include <sake/core/range/private/facade/iter_at_enable.hpp>
#include <sake/core/range/private/facade/sub_enable.hpp>
#include <sake/core/range/private/facade/traits.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

template< class Derived, class Params >
class facade
    : public private_::front_back_base< Derived, Params >
{
    typedef private_::front_back_base< Derived, Params > front_back_base_;
protected:
    typedef Derived derived_type;
public:
    using front_back_base_::derived;

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename facade,
        (( front_back_base_ ))
    )
    SAKE_MEMBERWISE_SWAP(
        typename facade,
        (( front_back_base_ ))
    )

private:
    typedef private_::traits< Params > traits_;
public:
    /***************************************************************************
     * typedef ... iterator;
     * typedef ... const_iterator;
     * typedef ... value_type;
     * typedef ... reference;
     * typedef ... const_reference;
     * typedef ... difference_type;
     * typedef ... size_type;
     * typedef ... range_traversal;
     **************************************************************************/
    SAKE_USING_TYPEDEF( typename traits_, iterator );
    SAKE_USING_TYPEDEF( typename traits_, const_iterator );
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, const_reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, size_type );
    SAKE_USING_TYPEDEF( typename traits_, range_traversal );

    template< class Introterminal >
    struct iterator_with
    { typedef typename sake::range::core_access::
        iterator_with< Derived, Introterminal >::type type; };
    template< class Introterminal >
    struct const_iterator_with
    { typedef typename sake::range::core_access::
        iterator_with< Derived const, Introterminal >::type type; };

    template< class T >
    typename private_::iter_at_enabler< Params, T, iterator >::type
    iter_at(T const & x)
    { return iter_at(x, sake::null_introterminal_tag()); }
    template< class T >
    typename private_::iter_at_enabler< Params, T, const_iterator >::type
    iter_at(T const & x) const
    { return iter_at(x, sake::null_introterminal_tag()); }

    template< class T, class Introterminal >
    typename private_::iter_at_lazy_enabler<
        Params, T, iterator_with< Introterminal > >::type
    iter_at(T const & x, Introterminal)
    { return sake::range::core_access::iter_at(derived(), x, Introterminal()); }
    template< class T, class Introterminal >
    typename private_::iter_at_lazy_enabler<
        Params, T, const_iterator_with< Introterminal > >::type
    iter_at(T const & x, Introterminal) const
    { return sake::range::core_access::iter_at(derived(), x, Introterminal()); }

    template< class Begin, class End >
    struct subrange_with
    { typedef typename sake::range::core_access::
        subrange_with< Derived, Begin, End >::type type; };
    template< class Begin, class End >
    struct const_subrange_with
    { typedef typename sake::range::core_access::
        subrange_with< Derived, Begin, End >::type type; };

    template< class Begin, class End >
    typename private_::sub_lazy_enabler<
        Params, Begin, End, subrange_with< Begin, End > >::type
    sub(Begin const & b, End const & e)
    { return sake::range::core_access::sub(derived(), b, e); }
    template< class Begin, class End >
    typename private_::sub_lazy_enabler<
        Params, Begin, End, const_subrange_with< Begin, End > >::type
    sub(Begin const & b, End const & e) const
    { return sake::range::core_access::sub(derived(), b, e); }

    template< class T >
    typename private_::at_enabler< Params, T, reference >::type
    at(T const x)
    { return sake::range::core_access::at(derived(), x); }
    template< class T >
    typename private_::at_enabler< Params, T, const_reference >::type
    at(T const x) const
    { return sake::range::core_access::at(derived(), x); }

    iterator
    begin()
    { return iter_at(sake::_begin); }
    const_iterator
    begin() const
    { return iter_at(sake::_begin); }

    iterator
    end()
    { return iter_at(sake::_end); }
    const_iterator
    end() const
    { return iter_at(sake::_end); }

    bool empty() const
    { return sake::range::core_access::empty(derived()); }
    inline friend bool
    range_empty(facade const & this_)
    { return this_.empty(); }

protected:
    template< class CDerived >
    struct iterator_of
    { typedef iterator type; };
    template< class CDerived >
    struct iterator_of< CDerived const >
    { typedef const_iterator type; };

    template< class CDerived, class Introterminal >
    struct iterator_with_of
    { typedef typename iterator_with< Introterminal >::type type; };
    template< class CDerived, class Introterminal >
    struct iterator_with_of< CDerived const, Introterminal >
    { typedef typename const_iterator_with< Introterminal >::type type; };

    template< class CDerived, class Begin, class End >
    struct subrange_with_of
    { typedef typename subrange_with< Begin, End >::type type; };
    template< class CDerived, class Begin, class End >
    struct subrange_with_of< CDerived const, Begin, End >
    { typedef typename const_subrange_with< Begin, End >::type type; };

    template< class CDerived >
    struct reference_of
    { typedef reference type; };
    template< class CDerived >
    struct reference_of< CDerived const >
    { typedef const_reference type; };

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename facade,
        (( front_back_base_ ))
    )

private:
    template< class T >
    struct explicit_constructor_enabler
        : private_::explicit_constructor_enabler< Derived, Params, T >
    { };
protected:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    explicit facade(T&& x,
        typename explicit_constructor_enabler<T>::type* = 0)
        : front_back_base_(sake::forward<T>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    explicit facade(T& x,
        typename explicit_constructor_enabler< T& >::type* = 0)
        : front_back_base_(x)
    { }

    template< class T >
    explicit facade(T const & x,
        typename explicit_constructor_enabler< T const & >::type* = 0)
        : front_back_base_(x)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class V >
    explicit facade(sake::emplacer< V ( ) >)
    { }

    template< class CDerived >
    static typename reference_of< CDerived >::type
    facade_at(CDerived& this_, sake::begin_tag)
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !this_.empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return *this_.begin();
    }

    template< class CDerived >
    static typename reference_of< CDerived >::type
    facade_at(CDerived& this_, sake::end_tag)
    {
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE( !this_.empty() );
#endif // #ifdef SAKE_RANGE_DEBUG
        return *sake::prior(this_.end());
    }

    template< class CDerived, class T >
    static typename reference_of< CDerived >::type
    facade_at(CDerived& this_, T const i)
    {
        BOOST_STATIC_ASSERT((
            boost_ext::is_convertible< T, difference_type >::value));
#ifdef SAKE_RANGE_DEBUG
        SAKE_REQUIRE_RELATION_ALL(
            (( sake::zero, <=, i ))
            (( i, <, this_.size() ))
        );
#endif // #ifdef SAKE_RANGE_DEBUG
        // TODO: This has the same issues as iterator::facade::operator[]
        // concerning returning a reference to a temporary...
        return this_.begin()[static_cast< difference_type >(i)];
    }

    bool facade_empty() const
    { return sake::range::default_impl::aux::empty(derived()); }
    difference_type facade_distance() const
    { return sake::range::default_impl::aux::distance(derived()); }
    size_type facade_size() const
    { return sake::range::default_impl::aux::size(derived()); }

    friend class sake::range::core_access;

    template< class CDerived, class T >
    static typename reference_of< CDerived >::type
    derived_at(CDerived& this_, T const x)
    { return facade_at(this_, x); }

    bool derived_empty() const
    { return facade_empty(); }
    difference_type derived_distance() const
    { return facade_distance(); }
    size_type derived_size() const
    { return facade_size(); }
};

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FACADE_HPP
