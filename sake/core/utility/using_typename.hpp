/*******************************************************************************
 * sake/core/utility/using_typename.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_USING_TYPENAME( scope, type )
 *
 * Expands to
 *     typedef typename scope :: type type ;
 * It is solely intended to reduce boilerplate.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_USING_TYPENAME_HPP
#define SAKE_CORE_UTILITY_USING_TYPENAME_HPP

#define SAKE_USING_TYPENAME( scope, type ) typedef typename scope :: type type ;

#endif // #ifndef SAKE_CORE_UTILITY_USING_TYPENAME_HPP
