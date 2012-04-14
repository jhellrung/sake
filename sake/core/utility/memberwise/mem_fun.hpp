/*******************************************************************************
 * sake/core/utility/memberwise/mem_fun.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_MEM_FUN[_R]( r, T, tag_seq, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/for_each.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/join.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/utility/memberwise/copy_assign.hpp>
#include <sake/core/utility/memberwise/copy_ctor.hpp>
#include <sake/core/utility/memberwise/default_ctor.hpp>
#include <sake/core/utility/memberwise/dtor.hpp>
#include <sake/core/utility/memberwise/hash_value.hpp>
#include <sake/core/utility/memberwise/move_assign.hpp>
#include <sake/core/utility/memberwise/move_ctor.hpp>
#include <sake/core/utility/memberwise/private/seq_transform_tuple_elem.hpp>
#include <sake/core/utility/memberwise/swap.hpp>

#define SAKE_MEMBERWISE_MEM_FUN( T, tag_seq, base_seq, member_seq ) \
    SAKE_MEMBERWISE_MEM_FUN_R( BOOST_PP_DEDUCE_R(), T, tag_seq, base_seq, member_seq )

#define SAKE_MEMBERWISE_MEM_FUN_R( r, T, tag_seq, base_seq, member_seq ) \
    SAKE_MEMBERWISE_MEM_FUN_R_impl( r, \
        T, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        tag_seq, \
        base_seq, \
        member_seq, \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 0, member_seq ), \
        SAKE_MEMBERWISE_seq_transform_tuple_elem( r, 1, member_seq ) \
    )
#define SAKE_MEMBERWISE_MEM_FUN_R_impl( r, \
    typenameT, T, tag_seq, base_seq, member_seq, member_type_seq, member_name_seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) ( \
        SAKE_MEMBERWISE_MEM_FUN_dispatch, \
        ( typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ), \
        tag_seq \
    )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch( r, data, elem ) \
    SAKE_MEMBERWISE_MEM_FUN_dispatch_impl( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_MEM_FUN_, elem ), \
        ( r, BOOST_PP_EXPAND( SAKE_BOOST_EXT_PP_TUPLE_REM6 data ) ) \
    )
#define SAKE_MEMBERWISE_MEM_FUN_dispatch_impl( macro, x ) macro x

#define SAKE_MEMBERWISE_MEM_FUN_copy_assign( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_assign_if_any_umc( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IF_ANY_UMC_R( r, typenameT, base_seq, member_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_assign_impl( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_ASSIGN_IMPL_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_copy_ctor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_COPY_CTOR_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_default_ctor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CTOR_R( r, T, SAKE_BOOST_EXT_PP_SEQ_JOIN( ( base_seq ) ( member_type_seq ) ) )
#define SAKE_MEMBERWISE_MEM_FUN_dtor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_DTOR( T )
#define SAKE_MEMBERWISE_MEM_FUN_hash_value( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_HASH_VALUE_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_move_assign( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_MOVE_ASSIGN_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_move_ctor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_MOVE_CTOR_R( r, T, base_seq, member_name_seq )
#define SAKE_MEMBERWISE_MEM_FUN_swap( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_MEMBERWISE_SWAP_R( r, T, base_seq, member_name_seq )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP
