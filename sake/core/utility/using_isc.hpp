/*******************************************************************************
 * core/utility/using_isc.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_USING_ISC( scope, type, name )
 *
 * Expands to
 *     static const type name = scope :: name ;
 * It is solely intended to reduce boilerplate.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_USING_ISC_HPP
#define SAKE_CORE_UTILITY_USING_ISC_HPP

#define SAKE_USING_ISC( scope, type, name ) static const type name = scope :: name ;

#endif // #ifndef SAKE_CORE_UTILITY_USING_ISC_HPP
