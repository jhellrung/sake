/*******************************************************************************
 * sake/core/utility/define_natural/private/tuple_elem_seq.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_TUPLE_ELEM_SEQ_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_TUPLE_ELEM_SEQ_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define SAKE_DEFINE_NATURAL_tuple_elem_seq( r, data, elem ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_tuple_elem_seq_, \
        BOOST_PP_IS_BINARY( elem ) \
    ) ( data, elem )

#define SAKE_DEFINE_NATURAL_tuple_elem_seq_0( data, elem ) ( elem )
#define SAKE_DEFINE_NATURAL_tuple_elem_seq_1( data, elem ) ( BOOST_PP_TUPLE_ELEM( 2, data, elem ) )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_TUPLE_ELEM_SEQ_HPP
