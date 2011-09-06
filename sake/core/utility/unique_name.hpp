/*******************************************************************************
 * core/utility/unique_name.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_UNIQUE_NAME( prefix )
 *
 * Expands to generate an identifier with the given prefix which is at least
 * unique to the line on which it expands.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_UNIQUE_NAME_HPP
#define SAKE_CORE_UTILITY_UNIQUE_NAME_HPP

#include <boost/preprocessor/cat.hpp>

#ifdef __COUNTER__

#define SAKE_UNIQUE_NAME( prefix ) BOOST_PP_CAT( prefix, __COUNTER__ )

#else // #ifdef __COUNTER__

#define SAKE_UNIQUE_NAME( prefix ) BOOST_PP_CAT( prefix, __LINE__ )

#endif // #ifdef __COUNTER__

#endif // #ifndef SAKE_CORE_UTILITY_UNIQUE_NAME_HPP
