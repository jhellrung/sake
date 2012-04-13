/*******************************************************************************
 * sake/core/utility/define_natural/hash_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_HASH_VALUE[_R]( [r,] T, base_seq, member_name_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/utility/hash_value.hpp>

#define SAKE_DEFINE_NATURAL_HASH_VALUE( T, base_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_HASH_VALUE_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_name_seq )

#define SAKE_DEFINE_NATURAL_HASH_VALUE_R( r, T, base_seq, member_name_seq ) \
    BOOST_PP_SEQ_CAT( \
        ( SAKE_DEFINE_NATURAL_HASH_VALUE_ ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_name_seq ) ) \
    ) ( r, T, base_seq, member_name_seq ) \
    inline friend ::std::size_t hash_value(T const & x) \
    { return x.hash_value(); }

#define SAKE_DEFINE_NATURAL_HASH_VALUE_00( r, T, base_seq, member_name_seq ) \
    ::std::size_t hash_value() const \
    { \
        ::std::size_t result = 0; \
        BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_base, ~, base_seq ) \
        BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_member, ~, member_name_seq ) \
        return result; \
    }

#define SAKE_DEFINE_NATURAL_HASH_VALUE_01( r, T, base_seq, member_name_seq ) \
    ::std::size_t hash_value() const \
    { \
        BOOST_PP_CAT( \
            SAKE_DEFINE_NATURAL_HASH_VALUE_01_, \
            BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( base_seq ) ) \
        ) ( r, T, base_seq ) \
    }
#define SAKE_DEFINE_NATURAL_HASH_VALUE_01_0( r, T, base_seq ) \
    ::std::size_t result = 0; \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_base, ~, base_seq ) \
    return result;
#define SAKE_DEFINE_NATURAL_HASH_VALUE_01_1( r, T, base_seq ) \
    return ::sake::hash_value(static_cast< SAKE_BOOST_EXT_PP_TUPLE_REM1 base_seq const & >(*this));

#define SAKE_DEFINE_NATURAL_HASH_VALUE_10( r, T, base_seq, member_name_seq ) \
    ::std::size_t hash_value() const \
    { \
        BOOST_PP_CAT( \
            SAKE_DEFINE_NATURAL_HASH_VALUE_10_, \
            BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_name_seq ) ) \
        ) ( r, T, member_name_seq ) \
    }
#define SAKE_DEFINE_NATURAL_HASH_VALUE_10_0( r, T, member_name_seq ) \
    ::std::size_t result = 0; \
    BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_HASH_VALUE_member, ~, member_name_seq ) \
    return result;
#define SAKE_DEFINE_NATURAL_HASH_VALUE_10_1( r, T, member_name_seq ) \
    return ::sake::hash_value(SAKE_BOOST_EXT_PP_TUPLE_REM1 member_name_seq );

#define SAKE_DEFINE_NATURAL_HASH_VALUE_11( r, T, base_seq, member_name_seq ) \
    static ::std::size_t hash_value() \
    { return static_cast< ::std::size_t >(0); }

#define SAKE_DEFINE_NATURAL_HASH_VALUE_base( r, data, elem ) \
    ::boost::hash_combine(result, static_cast< elem const & >(*this));
#define SAKE_DEFINE_NATURAL_HASH_VALUE_member( r, data, elem ) \
    ::boost::hash_combine(result, elem)

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_HASH_VALUE_HPP
