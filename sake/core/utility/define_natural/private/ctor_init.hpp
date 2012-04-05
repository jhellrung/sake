/*******************************************************************************
 * sake/core/utility/define_natural/private/ctor_init.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_ctor_init( r, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_CTOR_INIT_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_CTOR_INIT_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/utility/define_natural/private/member_name.hpp>
#include <sake/core/utility/inherit_cast.hpp>

#define SAKE_DEFINE_NATURAL_ctor_init( r, base_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_init_bases_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) \
    ) ( r, base_seq, member_seq, : BOOST_PP_EMPTY ) \

#define SAKE_DEFINE_NATURAL_init_bases_0( r, base_seq, member_seq, delimiter ) \
    delimiter() BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_DEFINE_NATURAL_comma_init_base, ~, base_seq ) \
    SAKE_DEFINE_NATURAL_init_bases_1( r, base_seq, member_seq, BOOST_PP_COMMA )
#define SAKE_DEFINE_NATURAL_comma_init_base( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    elem(::sake::move(::sake::inherit_cast< elem >(other)))

#define SAKE_DEFINE_NATURAL_init_bases_1( r, base_seq, member_seq, delimiter ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_init_members_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, member_seq, delimiter )

#define SAKE_DEFINE_NATURAL_init_members_0( r, member_seq, delimiter ) \
    delimiter() BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_DEFINE_NATURAL_comma_init_member, ~, member_seq )
#define SAKE_DEFINE_NATURAL_comma_init_member( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    SAKE_DEFINE_NATURAL_init_member_impl( \
        SAKE_DEFINE_NATURAL_member_name( elem ) )
#define SAKE_DEFINE_NATURAL_init_member_impl( member ) \
    member(::sake::move(other.member))

#define SAKE_DEFINE_NATURAL_init_members_1( r, member_seq, delimiter )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_PRIVATE_CTOR_INIT_HPP
