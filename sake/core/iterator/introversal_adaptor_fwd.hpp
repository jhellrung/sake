/*******************************************************************************
 * sake/core/iterator/introversal_adaptor_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP
#define SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP

#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

template<
    class I,
    bool ExplicitBegin, bool ExplicitEnd,
    class IntroversalMask = sake::null_introversal_tag
>
class introversal_adaptor;

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_INTROVERSAL_ADAPTOR_FWD_HPP
