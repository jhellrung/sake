/*******************************************************************************
 * sake/core/memberwise/hash_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_HASH_VALUE[_R]( [r,] T, member_seq )
 *
 * Expands to define a hash_value member function and friend function equivalent
 * to member-wise hash_value and combined via boost::hash_combine.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_HASH_VALUE_HPP
#define SAKE_CORE_MEMBERWISE_HASH_VALUE_HPP

#include <cstddef>

#include <boost/functional/hash.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/size_01x.hpp>

#include <sake/core/utility/define_friend_function.hpp>
#include <sake/core/utility/hash_value.hpp>

#define SAKE_MEMBERWISE_HASH_VALUE( T, member_seq ) \
    SAKE_MEMBERWISE_HASH_VALUE_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_HASH_VALUE_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_HASH_VALUE_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_HASH_VALUE_impl( r, T, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_HASH_VALUE_impl, \
        SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( member_seq, x ) \
    ) ( r, member_seq ) \
    SAKE_DEFINE_FRIEND_HASH_VALUE( T )

#define SAKE_MEMBERWISE_HASH_VALUE_impl0( r, member_seq ) \
    static std::size_t hash_value() \
    { return 0; }

#define SAKE_MEMBERWISE_HASH_VALUE_impl1( r, member_seq ) \
    ::std::size_t hash_value() const \
    { SAKE_MEMBERWISE_HASH_VALUE_impl1_impl( \
        BOOST_PP_SEQ_SIZE( BOOST_PP_SEQ_HEAD( member_seq ) ), \
        BOOST_PP_SEQ_TO_TUPLE( BOOST_PP_SEQ_HEAD( member_seq ) ) \
    ) }
#define SAKE_MEMBERWISE_HASH_VALUE_impl1_impl( n, member_spec ) \
    SAKE_MEMBERWISE_HASH_VALUE_impl1_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_HASH_VALUE_impl1_, n ), \
        member_spec \
    )
#define SAKE_MEMBERWISE_HASH_VALUE_impl1_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_HASH_VALUE_impl1_1( base ) \
    return ::sake::hash_value(static_cast< base const & >(*this));
#define SAKE_MEMBERWISE_HASH_VALUE_impl1_2( type, name ) \
    return ::sake::hash_value(name);

#define SAKE_MEMBERWISE_HASH_VALUE_implx( r, member_seq ) \
    ::std::size_t hash_value() const \
    { \
        std::size_t result = 0; \
        BOOST_PP_SEQ_FOR_EACH_R( r, \
            SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member, ~, member_seq ) \
        return result; \
    }
#define SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member( r, data, elem ) \
    SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_impl( \
        BOOST_PP_SEQ_SIZE( elem ), \
        BOOST_PP_SEQ_TO_TUPLE( elem ) \
    )
#define SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_impl( n, member_spec ) \
    SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_, n ), \
        member_spec \
    )
#define SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_1( base ) \
    ::boost::hash_combine(result, static_cast< base const & >(*this));
#define SAKE_MEMBERWISE_HASH_VALUE_hash_combine_member_2( type, name ) \
    ::boost::hash_combine(result, name);

#endif // #ifndef SAKE_CORE_MEMBERWISE_HASH_VALUE_HPP
