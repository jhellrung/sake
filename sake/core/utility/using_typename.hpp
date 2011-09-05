/*******************************************************************************
 * core/utility/using_typename.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_USING_TYPENAME( Scope, Type )
 *
 * Expands to
 *     typedef typename Scope :: Type Type ;
 * It is solely intended to reduce boilerplate.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_USING_TYPENAME_HPP
#define SAKE_CORE_UTILITY_USING_TYPENAME_HPP

#define SAKE_USING_TYPENAME( Scope, Type ) typedef typename Scope :: Type Type ;

#endif // #ifndef SAKE_CORE_UTILITY_USING_TYPENAME_HPP
