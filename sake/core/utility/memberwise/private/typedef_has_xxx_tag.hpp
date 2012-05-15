/*******************************************************************************
 * sake/core/utility/memberwise/private/typedef_has_xxx_tag.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_typedef_has_xxx_tag( r, member_seq, has_xxx )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#define SAKE_MEMBERWISE_typedef_has_xxx_tag( r, member_seq, has_xxx ) \
    static bool const BOOST_PP_CAT( has_xxx, _tag_value ) = \
        BOOST_PP_CAT( \
            SAKE_MEMBERWISE_has_xxx_tag_value_, \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
        ) ( r, member_seq, has_xxx ); \
    typedef ::boost::integral_constant< \
        bool, BOOST_PP_CAT( has_xxx, _tag_value ) \
    > BOOST_PP_CAT( has_xxx, _tag ); \
    template< class, class > friend class \
    ::sake::BOOST_PP_SEQ_CAT( ( has_type_ ) ( has_xxx ) ( _tag ) );

#define SAKE_MEMBERWISE_has_xxx_tag_value_0( r, member_seq, has_xxx ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_has_xxx_tag_value_0, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_seq ) ) \
    ) ( r, member_seq, has_xxx )
#define SAKE_MEMBERWISE_has_xxx_tag_value_00( r, member_seq, has_xxx ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( and, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_comma_has_trait, has_xxx, member_seq ) \
    >::value
#define SAKE_MEMBERWISE_comma_has_trait( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::data< BOOST_PP_SEQ_HEAD( elem ) >
#define SAKE_MEMBERWISE_has_xxx_tag_value_01( r, member_seq, has_xxx ) \
    ::sake::has_xxx< BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >::value

#define SAKE_MEMBERWISE_has_xxx_tag_value_1( r, member_seq, has_xxx ) \
    true

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_TYPEDEF_HAS_XXX_TAG_HPP
