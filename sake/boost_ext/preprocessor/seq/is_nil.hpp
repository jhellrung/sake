/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/is_nil.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq )
 *
 * Expands to 1 iff the given Boost.PP sequence is BOOST_PP_SEQ_NIL (or,
 * technically, any preprocessor token that "disappears" when a 1-tuple is
 * appended to it).
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_IS_NIL_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_IS_NIL_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/logical/compl.hpp>
#include <boost/preprocessor/seq/size.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq ) \
    BOOST_PP_COMPL( BOOST_PP_BOOL( BOOST_PP_DEC( BOOST_PP_SEQ_SIZE( seq ( ~ ) ) ) ) )

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_IS_NIL_HPP
