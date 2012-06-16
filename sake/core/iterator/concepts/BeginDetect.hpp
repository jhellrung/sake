/*******************************************************************************
 * sake/core/iterator/concepts/BeginDetect.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::BeginDetect<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_BEGIN_DETECT_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_BEGIN_DETECT_HPP

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/IntroversalBase.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template< class I >
struct BeginDetect
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_introversal_tag >
{ };

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_BEGIN_DETECT_HPP
