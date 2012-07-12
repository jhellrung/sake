/*******************************************************************************
 * sake/core/memberwise/destructor_tags.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DESTRUCTOR_TAGS[_R]( [r,] member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_MEMBERWISE_DESTRUCTOR_TAGS_HPP
#define SAKE_CORE_MEMBERWISE_DESTRUCTOR_TAGS_HPP

#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/type_traits/has_nothrow_destructor.hpp>
#include <sake/core/type_traits/has_trivial_destructor.hpp>

#define SAKE_MEMBERWISE_DESTRUCTOR_TAGS( member_seq ) \
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS_R( BOOST_PP_DEDUCE_R(), member_seq )

#define SAKE_MEMBERWISE_DESTRUCTOR_TAGS_R( r, member_seq ) \
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG_R( r, \
        member_seq, \
        ( has_nothrow_destructor ) \
        ( has_trivial_destructor ) \
    )

#endif // #ifndef SAKE_CORE_MEMBERWISE_DESTRUCTOR_TAGS_HPP
