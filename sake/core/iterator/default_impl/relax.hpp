/*******************************************************************************
 * sake/core/iterator/default_impl/relax.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_RELAX_HPP
#define SAKE_CORE_ITERATOR_DEFAULT_IMPL_RELAX_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_template.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
    has_template_relax,
    relax, 1
)

namespace relax_private
{

template<
    class I,
    bool = sake::iterator::default_impl::has_template_relax<I>::value
>
struct dispatch;

template< class I >
struct dispatch< I, false >
{
    template< class Introterminal >
    struct apply
    { typedef I type; };
};

template< class I >
struct dispatch< I, true >
{
    template< class Introterminal >
    struct apply
    { typedef typename I::template relax< Introterminal >::type type; };
};

} // namespace relax_private

template< class I, class Introterminal >
struct relax
{ typedef typename relax_private::dispatch<I>::template
    apply< Introterminal >::type type; };

} // namespace default_impl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_RELAX_HPP
