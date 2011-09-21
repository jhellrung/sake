/*******************************************************************************
 * sake/core/utility/threaded_tag.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct single_threaded_tag
 * struct multi_threaded_tag
 *
 * These empty structs may be used as type template parameters to specify
 * thread-related interfaces and/or semantics.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_THREADED_TAG_HPP
#define SAKE_CORE_UTILITY_THREADED_TAG_HPP

namespace sake
{

struct single_threaded_tag { };
struct multi_threaded_tag { };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_THREADED_TAG_HPP
