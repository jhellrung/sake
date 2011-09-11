/*******************************************************************************
 * boost_ext/exception/define_exception.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_DEFINE_EXCEPTION( scope, name )
 *
 * This macro reduces the boilerplate necessary to define exception classes
 * compatible with Boost.Exception and with a (minimally) informative what().
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_EXCEPTION_DEFINE_EXCEPTION_HPP
#define SAKE_BOOST_EXT_EXCEPTION_DEFINE_EXCEPTION_HPP

#include <exception>

#include <boost/exception/exception.hpp>
#include <boost/preprocessor/stringize.hpp>

#define SAKE_BOOST_EXT_DEFINE_EXCEPTION( scope, name ) \
    struct name \
        : virtual ::std::exception, \
          virtual ::boost::exception \
    { \
        const char* what() const throw ( ) \
        { return BOOST_PP_STRINGIZE( scope ) "::" BOOST_PP_STRINGIZE( name ); } \
    };

#endif // #ifndef SAKE_BOOST_EXT_EXCEPTION_DEFINE_EXCEPTION_HPP
