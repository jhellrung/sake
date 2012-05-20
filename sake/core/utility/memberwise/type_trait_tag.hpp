/*******************************************************************************
 * sake/core/utility/memberwise/private/type_trait_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG[_R]( [r,] member_seq, trait )
 * #define SAKE_MEMBERWISE_TYPE_TRAIT_TAG[_R]( [r,] member_seq, trait )
 * #define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE[_R]( [r,] member_seq, trait )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/size_01x.hpp>

#define SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG( member_seq, trait ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( BOOST_PP_DEDUCE_R(), member_seq, trait )
#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG( member_seq, trait ) \
    SAKE_MEMBERWISE_TYPE_TRAIT_TAG_R( BOOST_PP_DEDUCE_R(), member_seq, trait )
#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE( member_seq, trait ) \
    SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_R( BOOST_PP_DEDUCE_R(), member_seq, trait )

#define SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, member_seq, trait ) \
    typedef SAKE_MEMBERWISE_TYPE_TRAIT_TAG_R( r, member_seq, trait ) \
        BOOST_PP_CAT( trait, _tag ); \
    template< class, class > friend class \
    ::sake::BOOST_PP_SEQ_CAT( ( has_type_ ) ( trait ) ( _tag ) );

#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_R( r, member_seq, trait ) \
    ::boost::integral_constant< bool, \
        SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_R( r, member_seq, trait ) >

#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_R( r, member_seq, trait ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_, \
        SAKE_BOOST_EXT_PP_SEQ_SIZE_01X( member_seq, x ) \
    ) ( r, member_seq, trait )

#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_0( r, member_seq, trait ) \
    true

#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_1( r, member_seq, trait ) \
    ::sake::trait< BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >::value

#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_x( r, member_seq, trait ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( and, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_comma_trait, trait, member_seq ) \
    >::value
#define SAKE_MEMBERWISE_TYPE_TRAIT_TAG_VALUE_comma_trait( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    ::sake::data< BOOST_PP_SEQ_HEAD( elem ) >

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP
