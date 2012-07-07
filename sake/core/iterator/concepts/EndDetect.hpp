/*******************************************************************************
 * sake/core/iterator/concepts/EndDetect.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::EndDetect<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_END_DETECT_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_END_DETECT_HPP

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/IntroterminalBase.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template< class I >
struct EndDetect
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_detect_introterminal_tag >
{ };

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_END_DETECT_HPP
