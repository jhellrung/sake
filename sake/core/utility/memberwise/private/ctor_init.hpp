/*******************************************************************************
 * sake/core/utility/memberwise/private/ctor_init.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_ctor_init( r, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_CTOR_INIT_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_CTOR_INIT_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>

#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/utility/inherit_cast.hpp>

#define SAKE_MEMBERWISE_ctor_init( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_ctor_init_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, member_seq )

#define SAKE_MEMBERWISE_ctor_init_0( r, member_seq ) \
    : BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_MEMBERWISE_ctor_init_comma_init_member, ~, member_seq )
#define SAKE_MEMBERWISE_ctor_init_comma_init_member( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    SAKE_MEMBERWISE_ctor_init_comma_init_member_impl( \
        BOOST_PP_SEQ_SIZE( elem ), \
        BOOST_PP_SEQ_TO_TUPLE( elem ) \
    )
#define SAKE_MEMBERWISE_ctor_init_comma_init_member_impl( n, member_spec ) \
    SAKE_MEMBERWISE_ctor_init_comma_init_member_expand( \
        BOOST_PP_CAT( SAKE_MEMBERWISE_ctor_init_comma_init_member_, n ), \
        member_spec \
    )
#define SAKE_MEMBERWISE_ctor_init_comma_init_member_expand( macro, x ) macro x
#define SAKE_MEMBERWISE_ctor_init_comma_init_member_1( base ) \
    base(::sake::move(::sake::inherit_cast< base >(other)))
#define SAKE_MEMBERWISE_ctor_init_comma_init_member_2( type, name ) \
    name(::sake::move(other.name))

#define SAKE_MEMBERWISE_ctor_init_1( r, member_seq )

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_PRIVATE_CTOR_INIT_HPP
