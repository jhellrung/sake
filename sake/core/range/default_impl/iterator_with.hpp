/*******************************************************************************
 * sake/core/range/default_impl/iterator_with.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/core/iterator/adapt_introterminal.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/introspection/has_template.hpp>

namespace sake
{

namespace range
{

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
    has_template_iterator_with,
    iterator_with, 1
)

SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
    has_template_const_iterator_with,
    const_iterator_with, 1
)

namespace iterator_with_private
{

template< class R >
struct dispatch_bool
    : sake::range::default_impl::has_template_iterator_with<R>
{ };
template< class R >
struct dispatch_bool< R const >
    : sake::range::default_impl::has_template_const_iterator_with<R>
{ };

template< class R, bool = dispatch_bool<R>::value >
struct dispatch;

template< class R >
struct dispatch< R, false >
{
    template< class Iterator, class Introterminal >
    struct apply
    {
        BOOST_STATIC_ASSERT((boost::is_same<
            typename sake::iterator_introterminal< Iterator >::type,
            sake::null_introterminal_tag
        >::value));
        typedef typename sake::iterator::adapt_introterminal<
            Iterator, Introterminal >::type type;
    };
};

template< class R >
struct dispatch< R, true >
{
    template< class Iterator, class Introterminal >
    struct apply
    { typedef typename R::template iterator_with< Introterminal >::type type; };
};

template< class R >
struct dispatch< R const, false >
    : dispatch< R, false >
{ };

template< class R >
struct dispatch< R const, true >
{
    template< class Iterator, class Introterminal >
    struct apply
    { typedef typename R::template
        const_iterator_with< Introterminal >::type type; };
};

} // namespace iterator_with_private

template< class R, class Iterator, class Introterminal >
struct iterator_with
    : iterator_with_private::dispatch<R>::template
          apply< Iterator, Introterminal >
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_HPP
