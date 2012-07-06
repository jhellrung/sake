/*******************************************************************************
 * sake/core/iterator/facade.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::facade< Derived, Params >
 *
 * Keyword parameters accepted in Params:
 * - iterator::keyword::value
 * - iterator::keyword::reference
 * - iterator::keyword::difference
 * - iterator::keyword::traversal
 * - iterator::keyword::introversal
 * - iterator::keyword::compare_enable
 * - iterator::keyword::difference_enable
 * - iterator::keyword::chained_base
 *
 * Implementations to be defined in Derived:
 * - Dereferenceable
 *     derived_dereference() const -> reference
 * - Incrementable
 *     derived_increment() -> void
 * - SinglePass
 *     derived_equal(Other other) const -> bool
 * - Bidirectional
 *     derived_decrement() -> void
 * - RandomAccess
 *     derived_advance_ip(difference_type n) -> void
 *     derived_advance(difference_type n) const -> Derived [defaulted]
 *     derived_difference(Other other) const -> difference_type
 *     derived_less(Other other) const -> bool [defaulted]
 *     derived_less_equal(Other other) const -> bool [defaulted]
 *     derived_cmp(Other other) const -> sign_t [defaulted]
 * - Introversal
 *     struct derived_relax< Introversal >
 *       { typedef ... type; };
 *     derived_at_ip(Other other) -> void
 *     derived_at(Other other, Introversal) const
 *         -> relax< Introversal >::type [defaulted]
 * - BeginDetect
 *     derived_equal(begin_tag) const -> bool
 * - BeginAccess
 *     derived_at_ip(begin_tag) -> void
 *     derived_at(begin_tag, Introversal) const
 *         -> relax< Introversal >::type [defaulted]
 * - EndDetect
 *     derived_equal(end_tag) const -> bool
 * - EndAccess
 *     derived_at_ip(end_tag) -> void
 *     derived_at(end_tag, Introversal) const
 *         -> relax< Introversal >::type [defaulted]
 * - RandomAccess + BeginAccess
 *     derived_difference(begin_tag) const -> difference_type
 * - RandomAccess + EndAccess
 *     derived_difference(end_tag) const -> difference_type
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_FACADE_HPP
#define SAKE_CORE_ITERATOR_FACADE_HPP

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/category.hpp>
#include <sake/core/iterator/private/facade/arrow_dispatch.hpp>
#include <sake/core/iterator/private/facade/at_enable.hpp>
#include <sake/core/iterator/private/facade/compare_enable.hpp>
#include <sake/core/iterator/private/facade/difference_enable.hpp>
#include <sake/core/iterator/private/facade/equal_enable.hpp>
#include <sake/core/iterator/private/facade/explicit_constructor_enable.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/iterator/private/facade/traversal_base.hpp>
#include <sake/core/iterator/private/is_convertible_relax.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/math/sign.hpp>
#include <sake/core/math/sign_t.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

template< class Derived, class Params >
class facade
    : public private_::traversal_base< Derived, Params >
{
    typedef private_::traversal_base< Derived, Params > traversal_base_;
protected:
    typedef Derived derived_type;
public:
    using traversal_base_::derived;

    SAKE_MEMBERWISE_SWAP( typename facade, (( traversal_base_ )) )

private:
    typedef private_::traits< Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_value_type );
public:
    /***************************************************************************
     * typedef ... value_type;
     * typedef ... reference;
     * typedef ... difference_type;
     * typedef ... iterator_traversal;
     * typedef ... iterator_introversal;
     **************************************************************************/
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, iterator_traversal );
    SAKE_USING_TYPEDEF( typename traits_, iterator_introversal );

    typedef typename sake::iterator::private_::category<
        iterator_traversal, facade_value_type
    >::type iterator_category;

    /***************************************************************************
     * Dereferenceable
     * operator*() const -> reference
     * typedef ... pointer
     * operator->() const -> pointer
     **************************************************************************/
    reference
    operator*() const
    { return sake::iterator::core_access::dereference(derived()); }
private:
    typedef private_::arrow_dispatch< reference > arrow_dispatch_;
public:
    typedef typename arrow_dispatch_::type pointer;
    pointer
    operator->() const
    { return arrow_dispatch_::apply(operator*()); }

    /***************************************************************************
     * Incrementable
     * operator++() -> Derived&
     * operator++(int) -> ...
     *
     * Bidirectional
     * operator--() -> Derived&
     * operator--(int) -> Derived
     *
     * RandomAccess
     * operator[](difference_type n) const -> ...
     * operator+=(difference_type n) -> Derived&
     * operator-=(difference_type n) -> Derived&
     * operator+(difference_type n) -> Derived
     * friend operator+(difference_type n, Derived const & this_) -> Derived
     * operator-(difference_type n) -> Derived
     *
     * Introversal
     * template< class Introversal > struct relax { typedef ... type; };
     * at_ip(T x) -> void
     * at(T x, Introversal) -> relax< Introversal >::type
     **************************************************************************/
 
    template< class T >
    typename private_::at_enabler< Derived, T >::type
    at_ip(T const & x)
    { sake::iterator::core_access::at_ip(derived(), x); }

    template< class T >
    typename private_::at_lazy_enabler<
        Derived, T,
        typename traversal_base_::template relax<>
    >::type
    at(T const & x) const
    { return at(x, sake::null_introversal_tag()); }

    template< class T, class Introversal >
    typename private_::at_lazy_enabler<
        Derived, T,
        typename traversal_base_::template relax< Introversal >
    >::type
    at(T const & x, Introversal) const
    { return sake::iterator::core_access::at(derived(), x, Introversal()); }
 
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename facade,
        (( traversal_base_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( traversal_base_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
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
        : traversal_base_(sake::forward<T>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    explicit facade(T& x,
        typename explicit_constructor_enabler< T& >::type* = 0)
        : traversal_base_(x)
    { }

    template< class T >
    explicit facade(T const & x,
        typename explicit_constructor_enabler< T const & >::type* = 0)
        : traversal_base_(x)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class V >
    explicit facade(sake::emplacer< V ( ) >)
    { }

    friend class sake::iterator::core_access;

    using traversal_base_::derived_equal;
    template< class Other >
    typename boost::enable_if_c<
        sake::iterator::private_::is_convertible_relax< Other, Derived >::value,
        bool
    >::type
    derived_equal(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) == sake::zero; }

    template< class Other >
    bool derived_less(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) < sake::zero; }
    template< class Other >
    bool derived_less_equal(Other const & other) const
    { return sake::iterator::core_access::cmp(derived(), other) <= sake::zero; }
    template< class Other >
    sake::sign_t derived_cmp(Other const & other) const
    { return sake::sign(derived() - other); }

    template< class Introversal >
    struct derived_relax
    { typedef Derived type; };

    template< class T, class Introversal >
    typename traversal_base_::template relax< Introversal >::type
    derived_at(T const & x, Introversal) const
    {
        typedef typename traversal_base_::template
            relax< Introversal >::type result_type;
        result_type result(derived());
        sake::iterator_traits< result_type >::at_ip(result, x);
        return result;
    }
};

/*******************************************************************************
 * facade relational operators
 ******************************************************************************/

#define function_prototype( x, y ) \
template< class D0, class P0, class D1, class P1 > \
inline typename private_::x ## _enabler< D0, P0, D1, P1 >::type \
y(sake::iterator::facade_adl::facade< D0, P0 > const & i0, \
  sake::iterator::facade_adl::facade< D1, P1 > const & i1)

function_prototype( equal, operator== )
{ return sake::iterator::core_access::equal(i0.derived(), i1.derived()); }
function_prototype( equal, operator!= )
{ return !(i0.derived() == i1.derived()); }

function_prototype( compare, operator< )
{ return sake::iterator::core_access::less(i0.derived(), i1.derived()); }
function_prototype( compare, operator> )
{ return  (i1.derived() < i0.derived()); }
function_prototype( compare, operator<= )
{ return !(i1.derived() < i0.derived()); }
function_prototype( compare, operator>= )
{ return !(i0.derived() < i1.derived()); }

function_prototype( cmp, cmp )
{ return sake::iterator::core_access::cmp(i0.derived(), i1.derived()); }

/*******************************************************************************
 * facade arithmetic operators
 ******************************************************************************/

function_prototype( difference, operator- )
{ return sake::iterator::core_access::difference(i0.derived(), i1.derived()); }

#undef function_prototype

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_FACADE_HPP
