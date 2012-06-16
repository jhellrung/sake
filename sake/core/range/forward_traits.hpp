/*******************************************************************************
 * sake/core/range/forward_traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_forward_traits<R>
 *
 * struct range_forward_iterator<R>
 * struct range_forward_reference<R>
 * struct range_forward_traversal<R>
 *
 * range_forward_traits has the following interface.
 *
 * template< class R >
 * struct range_forward_traits
 * {
 *     typedef ... value_type;
 *     typedef ... difference_type;
 *     typedef ... size_type;
 *
 *     typedef ... base_reference;
 *     typedef ... base_traversal;
 *
 *     template< class Introversal >
 *     struct base_iterator_with { typedef ... type; };
 *
 *     static bool
 *     empy(R const & r);
 *     static size_type
 *     size(R const & r);
 *     static difference_type
 *     distance(R const & r);
 *
 *     typedef ... reference;
 *     typedef ... traversal;
 *
 *     template< class Introversal >
 *     struct iterator_with { typedef ... type; };
 *
 *     template< class T, class Introversal >
 *     static typename iterator_with< Introversal >::type
 *     iter_at(R&& r, T x, Introversal);
 *
 *     template< class I >
 *     static reference
 *     at(R&& r, I i);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_HPP
#define SAKE_CORE_RANGE_FORWARD_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/move_iterator.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/range/has_value_ownership.hpp>
#include <sake/core/range/move_iterator.hpp>
#include <sake/core/range/traits.hpp>

namespace sake
{

template< class R >
struct range_forward_traits;

template< class R, class Introversal = sake::null_introversal_tag >
struct range_forward_iterator
    : sake::range_forward_traits<R>::template iterator_with< Introversal >
{ };

template< class R >
struct range_forward_reference
{ typedef typename sake::range_forward_traits<R>::reference type; };

template< class R >
struct range_forward_traversal
{ typedef typename sake::range_forward_traits<R>::traversal type; };

namespace range_forward_traits_private
{

template< class R >
struct dispatch_bool
{
    typedef typename sake::range_reference<R>::type reference;
#ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value = boost_ext::mpl::and2<
        sake::range_has_value_ownership<R>,
        boost_ext::is_lvalue_reference_to_nonconst< reference >
    >::value;
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value = boost_ext::mpl::and3<
        sake::range_has_value_ownership<R>,
        boost_ext::is_lvalue_reference_to_nonconst< reference >,
        sake::has_move_emulation<
            typename boost_ext::remove_reference< reference >::type >
    >::value;
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

template< class R, bool = dispatch_bool<R>::value >
struct dispatch;

template< class R >
struct impl
    : dispatch<R>
{ };

template< class R >
struct dispatch< R, false >
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introversal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introversal >
    { };

    template< class T, class Introversal >
    static typename base_iterator_with< Introversal >::type
    iter_at(SAKE_RV_REF( R ) r, T const & x, Introversal)
    { return base_traits::iter_at(SAKE_AS_LVALUE(r), x, Introversal()); }

    template< class T >
    static base_reference
    at(SAKE_RV_REF( R ) r, T const x)
    { return base_traits::at(SAKE_AS_LVALUE(r), x); }
};

template< class R >
struct dispatch< R, true >
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introversal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introversal >
    { };

    typedef sake::move_iterator< base_iterator > iterator;
    typedef typename iterator::reference reference;
    typedef typename iterator::iterator_traversal traversal;

    template< class Introversal >
    struct iterator_with
    {
        typedef sake::move_iterator<
            typename base_iterator_with< Introversal >::type
        > type;
    };

    template< class T, class Introversal >
    static typename iterator_with< Introversal >::type
    iter_at(SAKE_RV_REF( R ) r, T const & x, Introversal)
    {
        return typename iterator_with< Introversalr >::type(
            base_traits::iter_at(SAKE_AS_LVALUE(r), x, Introversal());
    }

    template< class T >
    static reference
    at(SAKE_RV_REF( R ) r, T const x)
    { return sake::move(base_traits::at(SAKE_AS_LVALUE(r), x)); }
};

template< class R >
struct impl< R& >
    : sake::range_traits<R>
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introversal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introversal >
    { };
};

} // namespace range_forward_traits_private

template< class R >
struct range_forward_traits
    : range_forward_traits_private::impl<
          typename boost_ext::remove_rvalue_reference<R>::type >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_HPP
