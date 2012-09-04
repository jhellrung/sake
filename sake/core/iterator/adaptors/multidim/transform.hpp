/*******************************************************************************
 * sake/core/iterator/adaptors/multidim/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_TRANSFORM_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_TRANSFORM_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/emplacer/construct.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/multidim_traits_fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/transform.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>

namespace sake
{

namespace extension
{

template< class I, class F, class Params >
struct iterator_multidim_traits<
    sake::iterator::adaptors::transform< I, F, Params >,
    typename boost::enable_if_c<
        sake::iterator_multidim_enable<I>::value >::type
>
{
    typedef boost::true_type enable_tag;

    typedef sake::iterator::adaptors::transform< I, F, Params > this_type;
    typedef sake::iterator_multidim_traits<I> base_traits;

    typedef typename base_traits::outer_iterator base_outer_iterator;
    typedef typename sake::iterator_reference<
        base_outer_iterator
    >::type base_inner_range;
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost_ext::is_reference< base_inner_range >,
        sake::range_is_adapt_by_value< base_inner_range >
    >::value));

    typedef sake::iterator::adaptors::transform<
        base_outer_iterator,
        sake::range::adapt::functional::transform< F, Params >
    > outer_iterator;
    typedef typename sake::iterator_reference<
        outer_iterator
    >::type inner_range;
    BOOST_STATIC_ASSERT((!boost_ext::is_reference< inner_range >::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or< inner_range >::value));

    template< class Introterminal >
    struct inner_iterator_with
        : inner_range::template iterator_with< Introterminal >
    { };

    static outer_iterator
    outer(this_type const & i)
    {
        return outer_iterator(
            base_traits::outer(i.base()),
            sake::construct::emplacer(i.function())
        );
    }

    template< class Introterminal >
    static typename inner_iterator_with< Introterminal >::type
    inner(this_type const & i, Introterminal)
    {
        return typename inner_iterator_with< Introterminal >::type(
            base_traits::inner(i.base(), Introterminal()),
            i.function()
        );
    }

    template< class Inner >
    static void
    at_ip(this_type& i, outer_iterator const & j, Inner const & k)
    { base_traits::at_ip(i.protected_base(), j.base(), k.base()); }

    template< class Inner, class Introterminal >
    static typename this_type::template relax< Introterminal >::type
    at(
        this_type const & i,
        outer_iterator const & j, Inner const & k,
        Introterminal)
    {
        return typename this_type::template relax< Introterminal >::type(
            base_traits::at(i.base(), j.base(), k.base(), Introterminal()),
            i.function()
        );
    }
};

} // namespace extension

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_MULTIDIM_TRANSFORM_HPP
