/*******************************************************************************
 * sake/core/range/default_impl/iterator_with.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_INTROVERSAL_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_INTROVERSAL_HPP

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/introversal_adaptor_relax.hpp>
#include <sake/core/introspection/has_template.hpp>
#include <sake/core/range/traits_fwd.hpp>

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

template<
    class R,
    bool = has_template_iterator_with<R>::value
>
struct dispatch;

template< class R >
struct dispatch< R, false >
{
    template< class Iterator, class Introversal >
    struct apply
        : sake::iterator::introversal_adaptor_relax< Iterator, Introversal >
    { };
};

template< class R >
struct dispatch< R, true >
{
    template< class Iterator, class Introversal >
    struct apply
        : R::template iterator_with< Introversal >
    { };
};

template<
    class R,
    bool = has_template_const_iterator_with<R>::value
>
struct dispatch_const;

template< class R >
struct dispatch_const< R, false >
{
    template< class Iterator, class Introversal >
    struct apply
        : sake::iterator::introversal_adaptor_relax< Iterator, Introversal >
    { };
};

template< class R >
struct dispatch_const< R, true >
{
    template< class Iterator, class Introversal >
    struct apply
        : R::template const_iterator_with< Introversal >
    { };
};

} // namespace iterator_with_private

template< class R, class Iterator, class Introversal >
struct iterator_with
    : iterator_with_private::dispatch<R>::template
          apply< Iterator, Introversal >
{ };

template< class R, class Iterator, class Introversal >
struct iterator_with< R const, Iterator, Introversal >
    : iterator_with_private::dispatch_const<R>::template
          apply< Iterator, Introversal >
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_ITERATOR_WITH_INTROVERSAL_HPP
