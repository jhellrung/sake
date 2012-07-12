/*******************************************************************************
 * sake/core/iterator/concepts/private/IntroterminalBase_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_FWD_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_FWD_HPP

#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

template<
    class I,
    class Introterminal = typename sake::iterator_introterminal<I>::type
>
struct IntroterminalBase;

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_FWD_HPP
