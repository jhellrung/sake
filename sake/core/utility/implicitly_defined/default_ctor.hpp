/*******************************************************************************
 * sake/core/utility/implicitly_defined/default_ctor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( T )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_DEFAULT_CTOR_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_DEFAULT_CTOR_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( T ) \
    T() = default;

#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#define SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( T ) \
    T() { }

#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_DEFAULT_CTOR_HPP
