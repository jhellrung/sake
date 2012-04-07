/*******************************************************************************
 * sake/boost_ext/preprocessor/seq/to_mpl_vector.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( seq )
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TO_MPL_VECTOR_HPP
#define SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TO_MPL_VECTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES

#include <sake/boost_ext/mpl/vector.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( seq ) \
    ::sake::boost_ext::mpl::vector< \
        BOOST_PP_IIF( \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq ), \
            BOOST_PP_TUPLE_EAT(1), \
            BOOST_PP_SEQ_ENUM \
        ) ( seq ) \
    >

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector/vector20.hpp>
#include <boost/preprocessor/cat.hpp>

#include <sake/boost_ext/preprocessor/seq/size.hpp>

#define SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( seq ) \
    ::boost::mpl::BOOST_PP_CAT( vector, SAKE_BOOST_EXT_PP_SEQ_SIZE( seq ) )< \
        BOOST_PP_IIF( \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( seq ), \
            BOOST_PP_TUPLE_EAT(1), \
            BOOST_PP_SEQ_ENUM \
        ) ( seq ) \
    >

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#endif // #ifndef SAKE_BOOST_EXT_PREPROCESSOR_SEQ_TO_MPL_VECTOR_HPP
