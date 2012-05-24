/*******************************************************************************
 * sake/core/iterator/private/adaptor/chained_base_constructible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CHAINED_BASE_CONSTRUCTIBLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CHAINED_BASE_CONSTRUCTIBLE_HPP

#include <sake/boost_ext/mpl/at.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/keyword.hpp>

namespace sake
{

namespace iterator_adaptor_private
{

template<
    class D, class P,
    class C = typename boost_ext::mpl::at<
        P, sake::iterator_keyword::tag::chained_base,
        void
    >::type
>
struct chained_base_constructible_dispatch
{
    typedef typename boost_ext::propagate_const<D,C>::type & type;
    static type
    apply(type x)
    { return x; }
};

template< class D, class P >
struct chained_base_constructible_dispatch< D, P, void >
{
    typedef sake::emplacer< void ( ) > type;
    template< class T >
    static type
    apply(T&)
    { return type(); }
};

template< class D, class P >
inline typename chained_base_constructible_dispatch<D,P>::type
chained_base_constructible(sake::iterator_facade<D,P>& i)
{ return chained_base_constructible_dispatch<D,P>::apply(i); }

template< class D, class P >
inline typename chained_base_constructible_dispatch< D const, P >::type
chained_base_constructible(sake::iterator_facade< D, P > const & i)
{ return chained_base_constructible_dispatch< D const, P >::apply(i); }

} // namespace iterator_adaptor_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_ADAPTOR_CHAINED_BASE_CONSTRUCTIBLE_HPP
