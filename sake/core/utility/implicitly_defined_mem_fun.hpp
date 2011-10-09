/*******************************************************************************
 * sake/core/utility/implicitly_defined_mem_fun.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_IMPLICITLY_DEFINED_MEM_FUN[_R]( r, T, tag_seq, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MEM_FUN_HPP
#define SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MEM_FUN_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/seq/for_each.hpp>

#include <sake/core/utility/implicitly_defined_copy_assign.hpp>
#include <sake/core/utility/implicitly_defined_default_ctor.hpp>
#include <sake/core/utility/implicitly_defined_dtor.hpp>
#include <sake/core/utility/implicitly_defined_move_assign.hpp>
#include <sake/core/utility/implicitly_defined_move_ctor.hpp>

#define SAKE_IMPLICITLY_DEFINED_MEM_FUN( T, tag_seq, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_MEM_FUN_R( BOOST_PP_DEDUCE_R(), T, tag_seq, base_seq, member_seq )

#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_R( r, T, tag_seq, base_seq, member_seq ) \
    BOOST_PP_CAT( SAKE_BOOST_EXT_PP_SEQ_FOR_EACH_, r ) ( \
        SAKE_IMPLICITLY_DEFINED_MEM_FUN_dispatch, \
        ( T, base_seq, member_seq ), \
        tag_seq \
    )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_dispatch( r, data, elem ) \
    BOOST_PP_CAT( SAKE_IMPLICITLY_DEFINED_MEM_FUN_, elem ) ( r, \
        BOOST_PP_TUPLE_ELEM( 3, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, data ) \
    )

#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_copy_assign( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_R( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_copy_assign_if_any_umc( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IF_ANY_UMC_R( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_copy_assign_impl( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_COPY_ASSIGN_IMPL_R( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_default_ctor( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( T )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_dtor( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_DTOR( T )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_move_assign( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_MOVE_ASSIGN_R( r, T, base_seq, member_seq )
#define SAKE_IMPLICITLY_DEFINED_MEM_FUN_move_ctor( r, T, base_seq, member_seq ) \
    SAKE_IMPLICITLY_DEFINED_MOVE_CTOR_R( r, T, base_seq, member_seq )

#endif // #ifndef SAKE_CORE_UTILITY_IMPLICITLY_DEFINED_MEM_FUN_HPP
