/*******************************************************************************
 * sake/core/range/default_impl/subrange_with.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_SUBRANGE_WITH_HPP
#define SAKE_CORE_RANGE_DEFAULT_IMPL_SUBRANGE_WITH_HPP

#include <sake/core/iterator/adapt_introversal.hpp>
#include <sake/core/introspection/has_template.hpp>
#include <sake/core/range/basic/fwd.hpp>
#include <sake/core/range/basic/subrange.hpp>
#include <sake/core/range/default_impl/iterator_with.hpp>
#include <sake/core/range/private/introversal_from_begin_end.hpp>

namespace sake
{

namespace range
{

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
    has_template_subrange_with,
    subrange_with, 2
)

SAKE_INTROSPECTION_DEFINE_HAS_TEMPLATE_PARAMS(
    has_template_const_subrange_with,
    const_subrange_with, 2
)

namespace subrange_with_private
{

template< class R, class Iterator, class Begin, class End >
class helper
{
    typedef typename sake::range::private_::
        introversal_from_begin_end< Begin, End >::type introversal;
public:
    typedef sake::range::basic::subrange<
        typename sake::iterator::adapt_introversal<
            typename sake::range::default_impl::iterator_with<
                R, Iterator, introversal >::type,
            introversal,
            introversal
        >::type
    > type;
};

template< class R >
struct dispatch_bool
    : sake::range::default_impl::has_template_subrange_with<R>
{ };
template< class R >
struct dispatch_bool< R const >
    : sake::range::default_impl::has_template_const_subrange_with<R>
{ };

template< class R, bool = dispatch_bool<R>::value >
struct dispatch;

template< class R >
struct dispatch< R, false >
{
    template< class Iterator, class Begin, class End >
    struct apply
        : subrange_with_private::helper< R, Iterator, Begin, End >
    { };
};

template< class R >
struct dispatch< R, true >
{
    template< class Iterator, class Begin, class End >
    struct apply
        : R::template subrange_with< Begin, End >
    { };
};

template< class R >
struct dispatch< R const, false >
{
    template< class Iterator, class Begin, class End >
    struct apply
        : subrange_with_private::helper< R const, Iterator, Begin, End >
    { };
};

template< class R >
struct dispatch< R const, true >
{
    template< class Iterator, class Begin, class End >
    struct apply
        : R::template const_subrange_with< Begin, End >
    { };
};

} // namespace subrange_with_private

template< class R, class Iterator, class Begin, class End >
struct subrange_with
    : subrange_with_private::dispatch<R>::template
          apply< Iterator, Begin, End >
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_DEFAULT_IMPL_SUBRANGE_WITH_HPP
