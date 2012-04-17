/*******************************************************************************
 * sake/core/utility/memberwise/mem_fun.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_MEM_FUN[_R]( r, typenameT, tag_seq, member_seq )
 *
 * Expands to memberwise definitions of a specified set of member functions.
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_MEM_FUN_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_MEM_FUN_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/for_each.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/utility/memberwise/copy_assign.hpp>
#include <sake/core/utility/memberwise/copy_ctor.hpp>
#include <sake/core/utility/memberwise/default_ctor.hpp>
#include <sake/core/utility/memberwise/dtor.hpp>
#include <sake/core/utility/memberwise/hash_value.hpp>
#include <sake/core/utility/memberwise/move_assign.hpp>
#include <sake/core/utility/memberwise/move_ctor.hpp>
#include <sake/core/utility/memberwise/swap.hpp>

#define SAKE_MEMBERWISE_MEM_FUN( T, tag_seq, member_seq ) \
    SAKE_MEMBERWISE_MEM_FUN_R( BOOST_PP_DEDUCE_R(), T, tag_seq, member_seq )

// For some reason, the MSVC preprocessor screws up when member_seq is
// BOOST_PP_SEQ_NIL; it might be a problem with the Boost.PP library, I'm not
// sure.  In any case, we have to work around it...
#define SAKE_MEMBERWISE_MEM_FUN_R( r, T, tag_seq, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_MEM_FUN_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, T, tag_seq, member_seq ) \

#define SAKE_MEMBERWISE_MEM_FUN_0( r, T, tag_seq, member_seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) ( \
        SAKE_MEMBERWISE_MEM_FUN_dispatch0, ( T, member_seq ), tag_seq )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch0( r, data, elem ) \
    SAKE_MEMBERWISE_MEM_FUN_dispatch_expand00( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_MEM_FUN_, elem ), \
        ( r, SAKE_BOOST_EXT_PP_TUPLE_REM2 data ) \
    )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch_expand00( macro, x ) \
    SAKE_MEMBERWISE_MEM_FUN_dispatch_expand01( macro, x )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch_expand01( macro, x ) macro x

#define SAKE_MEMBERWISE_MEM_FUN_1( r, T, tag_seq, member_seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) ( \
        SAKE_MEMBERWISE_MEM_FUN_dispatch1, T, tag_seq )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch1( r, data, elem ) \
    BOOST_PP_CAT( SAKE_MEMBERWISE_MEM_FUN_, elem ) ( r, data, BOOST_PP_SEQ_NIL )

#define SAKE_MEMBERWISE_MEM_FUN_copy_assign( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_assign_if_any_umc( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_assign_impl( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_ctor( r, T, member_seq ) \
    SAKE_MEMBERWISE_COPY_CTOR_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_default_ctor( r, T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CTOR_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_dtor( r, T, member_seq ) \
    SAKE_MEMBERWISE_DTOR_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_hash_value( r, T, member_seq ) \
    SAKE_MEMBERWISE_HASH_VALUE_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_move_assign( r, T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_move_ctor( r, T, member_seq ) \
    SAKE_MEMBERWISE_MOVE_CTOR_R( r, T, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_swap( r, T, member_seq ) \
    SAKE_MEMBERWISE_SWAP_R( r, T, member_seq )

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_MEM_FUN_HPP
