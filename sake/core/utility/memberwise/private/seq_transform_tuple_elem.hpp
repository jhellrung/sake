/*******************************************************************************
 * sake/core/utility/memberwise/private/seq_transform_tuple_elem.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_SEQ_TRANSFORM_TUPLE_ELEM_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_SEQ_TRANSFORM_TUPLE_ELEM_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#define SAKE_MEMBERWISE_seq_transform_tuple_elem( r, i, tuple_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_seq_transform_tuple_elem_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( tuple_seq ) \
    ) ( r, i, tuple_seq )

#define SAKE_MEMBERWISE_seq_transform_tuple_elem_0( r, i, tuple_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_MEMBERWISE_tuple_elem_seq, i, tuple_seq )
#define SAKE_MEMBERWISE_seq_transform_tuple_elem_1( r, i, tuple_seq ) \
    BOOST_PP_SEQ_NIL

#define SAKE_MEMBERWISE_tuple_elem_seq( r, data, elem ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_tuple_elem_seq_, \
        BOOST_PP_IS_BINARY( elem ) \
    ) ( data, elem )
#define SAKE_MEMBERWISE_tuple_elem_seq_0( data, elem ) ( elem )
#define SAKE_MEMBERWISE_tuple_elem_seq_1( data, elem ) ( BOOST_PP_TUPLE_ELEM( 2, data, elem ) )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_SEQ_TRANSFORM_TUPLE_ELEM_HPP
