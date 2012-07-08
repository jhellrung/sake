/*******************************************************************************
 * sake/core/range/forward_traits_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_FWD_HPP
#define SAKE_CORE_RANGE_FORWARD_TRAITS_FWD_HPP

#include <sake/core/iterator/categories.hpp>

namespace sake
{

template< class R >
struct range_forward_traits;

template< class R, class Introterminal = sake::null_introterminal_tag >
struct range_forward_iterator
{ typedef typename sake::range_forward_traits<R>::template
    iterator_with< Introterminal >::type type; };

template< class R, class Begin = void, class End = void >
struct range_forward_subrange
{ typedef typename sake::range_forward_traits<R>::template
    subrange_with< Begin, End >::type type; };

template< class R >
struct range_forward_reference
{ typedef typename sake::range_forward_traits<R>::reference type; };

template< class R >
struct range_forward_traversal
{ typedef typename sake::range_forward_traits<R>::traversal type; };

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_FWD_HPP
