/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/enum.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_ENUM( seq )
 *
 * Expands to nothing if seq is BOOST_PP_SEQ_NIL.  Otherwise, expands to
 * BOOST_PP_SEQ_ENUM( seq ).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ENUM_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ENUM_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_ENUM( seq ) \
    BOOST_PP_CAT( \
        SAKE_BOOST_EXT_PP_SEQ_ENUM_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq ) \
    ) ( seq )
#define SAKE_BOOST_EXT_PP_SEQ_ENUM_0( seq ) BOOST_PP_SEQ_ENUM( seq )
#define SAKE_BOOST_EXT_PP_SEQ_ENUM_1( seq )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_ENUM_HPP
