/*******************************************************************************
 * sake/core/iterator/begin_end_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_BEGIN_END_TAG_HPP
#define SAKE_CORE_ITERATOR_BEGIN_END_TAG_HPP

namespace sake
{

struct begin_tag { };
struct end_tag { };

static sake::begin_tag const _begin = { };
static sake::end_tag const _end = { };

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_BEGIN_END_TAG_HPP
