/*******************************************************************************
 * sake/core/iterator/adapt_introterminal_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPT_INTROTERMINAL_FWD_HPP
#define SAKE_CORE_ITERATOR_ADAPT_INTROTERMINAL_FWD_HPP

#include <boost/mpl/set/set0.hpp>

#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

template<
    class I, class Introterminal,
    class IntroterminalMask = sake::null_introterminal_tag
>
class adapt_introterminal;

namespace adaptors
{

template<
    class I,
    class Tags = boost::mpl::set0<>,
    class IntroterminalMask = sake::null_introterminal_tag
>
class introterminal;

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPT_INTROTERMINAL_FWD_HPP
