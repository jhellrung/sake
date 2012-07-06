/*******************************************************************************
 * sake/core/iterator/introversal_adaptor_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP
#define SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP

#include <boost/mpl/set/set0.hpp>

#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

template<
    class I, class Introversal,
    class IntroversalMask = sake::null_introversal_tag
>
class adapt_introversal;

namespace adaptors
{

template<
    class I,
    class Tags = boost::mpl::set0<>,
    class IntroversalMask = sake::null_introversal_tag
>
class introversal;

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP
