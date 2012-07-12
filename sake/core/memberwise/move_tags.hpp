/*******************************************************************************
 * sake/core/memberwise/move_tags.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_MOVE_TAGS[_R]( [r,] member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_MOVE_CONSTRUCTOR_TAGS_HPP
#define SAKE_CORE_MEMBERWISE_MOVE_CONSTRUCTOR_TAGS_HPP

#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/type_traits/has_move_constructor.hpp>
#include <sake/core/type_traits/has_nothrow_move_assign.hpp>
#include <sake/core/type_traits/has_nothrow_move_constructor.hpp>

#define SAKE_MEMBERWISE_MOVE_TAGS( member_seq ) \
    SAKE_MEMBERWISE_MOVE_TAGS_R( BOOST_PP_DEDUCE_R(), member_seq )

#define SAKE_MEMBERWISE_MOVE_TAGS_R( r, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, \
        member_seq, \
        ( has_move_constructor ) \
        ( has_nothrow_move_constructor ) \
        ( has_nothrow_move_assign ) \
    )

#endif // #ifndef SAKE_CORE_MEMBERWISE_MOVE_CONSTRUCTOR_TAGS_HPP
