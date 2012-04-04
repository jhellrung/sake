/*******************************************************************************
 * sake/core/utility/define_natural/hash_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_HASH_VALUE[_R]( [r,] T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/facilities/apply.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/eat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/hash_value.hpp>

#define SAKE_DEFINE_NATURAL_HASH_VALUE( T, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_HASH_VALUE_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_HASH_VALUE_R( r, T, base_seq, member_seq ) \
    std::size_t hash_value() const \
    { BOOST_PP_SEQ_CAT( \
        ( SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch ), \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ), \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) ) \
    ) ( r, T, base_seq, member_seq ) }

#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch00( r, T, base_seq, member_seq ) \
    std::size_t result = 0; \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_base, ~, base_seq ) \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_member, ~, member_seq ) \
    return result;

#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch01( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch01_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( base_seq ) ), \
    ) ( r, T, base_seq )
#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch01_0( r, T, base_seq ) \
    std::size_t result = 0; \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_base, ~, base_seq ) \
    return result;
#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch01_1( r, T, base_seq ) \
    return ::sake::hash_value(static_cast< BOOST_PP_APPLY( base_seq ) const & >(*this));

#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch10( r, T, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch10_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_seq ) ), \
    ) ( r, T, member_seq )
#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch10_0( r, T, member_seq ) \
    std::size_t result = 0; \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_member, ~, member_seq ) \
    return result;
#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch10_1( r, T, member_seq ) \
    return ::sake::hash_value(BOOST_PP_APPLY( member_seq ));

#define SAKE_DEFINE_NATURAL_HASH_VALUE_dispatch11( r, T, base_seq, member_seq ) \
    return 0;

#define SAKE_DEFINE_NATURAL_HASH_VALUE_base( r, data, elem ) \
    ::boost::hash_combine(result, static_cast< elem const & >(*this));
#define SAKE_DEFINE_NATURAL_HASH_VALUE_member( r, data, elem ) \
    ::boost::hash_combine( \
        result, \
        BOOST_PP_IIF( \
            BOOST_PP_IS_BINARY( elem ), \
            BOOST_PP_TUPLE_ELEM, \
            elem BOOST_PP_TUPLE_EAT(3) \
        ) ( 2, 1, elem ) \
    )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP
