/*******************************************************************************
 * sake/core/memberwise/copy_assign_tags.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_COPY_ASSIGN_TAGS[_R]( [r,] member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_COPY_ASSIGN_TAGS_HPP
#define SAKE_CORE_MEMBERWISE_COPY_ASSIGN_TAGS_HPP

#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/type_traits/has_nothrow_copy_assign.hpp>
#include <sake/core/type_traits/has_trivial_copy_assign.hpp>

#define SAKE_MEMBERWISE_COPY_ASSIGN_TAGS( member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_TAGS_R( BOOST_PP_DEDUCE_R(), member_seq )

#define SAKE_MEMBERWISE_COPY_ASSIGN_TAGS_R( r, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, \
        member_seq, \
        ( has_nothrow_copy_assign ) \
        ( has_trivial_copy_assign ) \
    )

#endif // #ifndef SAKE_CORE_MEMBERWISE_COPY_ASSIGN_TAGS_HPP
