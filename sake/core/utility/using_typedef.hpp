/*******************************************************************************
 * sake/core/utility/using_typedef.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_USING_TYPEDEF( scope, type )
 *
 * Expands to
 *     typedef scope :: type type
 * It is solely intended to reduce boilerplate.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_USING_TYPEDEF_HPP
#define SAKE_CORE_UTILITY_USING_TYPEDEF_HPP

#define SAKE_USING_TYPEDEF( scope, type ) typedef scope :: type type

#endif // #ifndef SAKE_CORE_UTILITY_USING_TYPEDEF_HPP
