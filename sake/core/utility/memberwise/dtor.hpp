/*******************************************************************************
 * sake/core/utility/memberwise/dtor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DTOR( T )
 * #define SAKE_NATURAL_DTOR_BODY()
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DTOR_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_DTOR_HPP

#include <boost/config.hpp>

#define SAKE_MEMBERWISE_DTOR( T ) \
    ~T() SAKE_NATURAL_DTOR_BODY()

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_NATURAL_DTOR_BODY() = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_NATURAL_DTOR_BODY() { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DTOR_HPP
