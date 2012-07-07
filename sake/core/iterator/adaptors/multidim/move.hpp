/*******************************************************************************
 * sake/core/iterator/adaptors/multidim/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * class iterator::adaptors::move<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_MOVE_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_MOVE_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/transform.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/move.hpp>

namespace sake
{

namespace extension
{

template< class I >
struct iterator_multidim_traits<
    sake::iterator::adaptors::move<I>,
    typename boost::enable_if_c<
        sake::iterator_multidim_enable<I>::value >::type
>
{
    typedef boost::true_type enable_tag;

    typedef sake::iterator::adaptors::move<I> this_type;
    typedef sake::iterator_multidim_traits<I> base_traits;

    typedef typename base_traits::outer_iterator base_outer_iterator;
    typedef typename sake::iterator_reference<
        base_outer_iterator
    >::type base_inner_range;
    typedef sake::range::adaptors::move< base_inner_range > inner_range;

    typedef sake::iterator::adaptors::transform<
        base_outer_iterator,
        sake::functional::construct< inner_range >
    > outer_iterator;

    template< class Introterminal >
    struct inner_iterator_with
        : inner_range::template iterator_with< Introterminal >
    { };

    static outer_iterator
    outer(this_type const & i)
    { return outer_iterator(base_traits::outer(i.base())); }

    template< class Introterminal >
    static typename inner_iterator_with< Introterminal >::type
    inner(this_type const & i, Introterminal)
    {
        return typename inner_iterator_with< Introterminal >::type(
            base_traits::inner(i.base(), Introterminal()));
    }

    template< class Inner >
    static void
    at_ip(this_type& i, outer_iterator const & j, Inner const & k)
    {
        base_traits::at_ip(i.protected_base(), j.base(), k.base());
        return i;
    }

    template< class Inner, class Introterminal >
    static typename this_type::template relax< Introterminal >::type
    at(
        this_type const & i,
        outer_iterator const & j, Inner const & k,
        Introterminal)
    {
        return typename this_type::template relax< Introterminal >::type(
            base_traits::at(i.base(), j.base(), k.base(), Introterminal()));
    }
};

} // namespace extension

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_MOVE_HPP
