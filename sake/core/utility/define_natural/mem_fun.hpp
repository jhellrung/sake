/*******************************************************************************
 * sake/core/utility/define_natural/mem_fun.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_MEM_FUN[_R]( r, T, tag_seq, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/eat.hpp>

#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/for_each.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/join.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/utility/define_natural/copy_assign.hpp>
#include <sake/core/utility/define_natural/default_ctor.hpp>
#include <sake/core/utility/define_natural/dtor.hpp>
#include <sake/core/utility/define_natural/hash_value.hpp>
#include <sake/core/utility/define_natural/move_assign.hpp>
#include <sake/core/utility/define_natural/move_ctor.hpp>
#include <sake/core/utility/define_natural/private/tuple_elem_seq.hpp>
#include <sake/core/utility/define_natural/swap.hpp>

#define SAKE_DEFINE_NATURAL_MEM_FUN( T, tag_seq, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_MEM_FUN_R( BOOST_PP_DEDUCE_R(), T, tag_seq, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_MEM_FUN_R( r, T, tag_seq, base_seq, member_seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) ( \
        SAKE_DEFINE_NATURAL_MEM_FUN_dispatch, \
        ( T, \
          SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
          base_seq, \
          member_seq, \
          BOOST_PP_IIF( \
              SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ), \
              BOOST_PP_SEQ_NIL BOOST_PP_TUPLE_EAT(4), \
              BOOST_PP_SEQ_FOR_EACH_R \
          ) ( r, SAKE_DEFINE_NATURAL_tuple_elem_seq, 0, member_seq ), \
          BOOST_PP_IIF( \
              SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ), \
              BOOST_PP_SEQ_NIL BOOST_PP_TUPLE_EAT(4), \
              BOOST_PP_SEQ_FOR_EACH_R \
          ) ( r, SAKE_DEFINE_NATURAL_tuple_elem_seq, 1, member_seq ) ), \
        tag_seq \
    )
#define SAKE_DEFINE_NATURAL_MEM_FUN_dispatch( r, data, elem ) \
    SAKE_DEFINE_NATURAL_MEM_FUN_dispatch_impl( \
        BOOST_PP_CAT( SAKE_DEFINE_NATURAL_MEM_FUN_, elem ), \
        ( r, BOOST_PP_EXPAND( SAKE_BOOST_EXT_PP_TUPLE_REM6 data ) ) \
    )
#define SAKE_DEFINE_NATURAL_MEM_FUN_dispatch_impl( macro, x ) macro x

#define SAKE_DEFINE_NATURAL_MEM_FUN_copy_assign( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_COPY_ASSIGN_R( r, T, base_seq, member_name_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_copy_assign_if_any_umc( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_COPY_ASSIGN_IF_ANY_UMC_R( r, typenameT, base_seq, member_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_copy_assign_impl( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_COPY_ASSIGN_IMPL_R( r, T, base_seq, member_name_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_default_ctor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_R( r, T, SAKE_BOOST_EXT_PP_SEQ_JOIN( ( base_seq ) ( member_type_seq ) ) )
#define SAKE_DEFINE_NATURAL_MEM_FUN_dtor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_DTOR( T )
#define SAKE_DEFINE_NATURAL_MEM_FUN_hash_value( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_HASH_VALUE_R( r, T, base_seq, member_name_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_move_assign( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_MOVE_ASSIGN_R( r, T, base_seq, member_name_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_move_ctor( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_MOVE_CTOR_R( r, T, base_seq, member_name_seq )
#define SAKE_DEFINE_NATURAL_MEM_FUN_swap( r, \
    typenameT, T, base_seq, member_seq, member_type_seq, member_name_seq ) \
    SAKE_DEFINE_NATURAL_SWAP_R( r, T, base_seq, member_name_seq )

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_MEM_FUN_HPP
