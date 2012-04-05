/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_SIZE( seq )
 *
 * Expands to 0 if seq is BOOST_PP_SEQ_NIL.  Otherwise, expands to
 * BOOST_PP_SEQ_SIZE( seq ).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/seq/size.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_SIZE( seq ) \
    BOOST_PP_DEC( BOOST_PP_SEQ_SIZE( seq ( ~ ) ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_SIZE_HPP
