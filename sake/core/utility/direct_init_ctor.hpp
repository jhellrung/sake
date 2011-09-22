/*******************************************************************************
 * sake/core/utility/direct_init_ctor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DIRECT_INIT_CTOR[_R]( [r,] T, member_seq )
 * #define SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS[_R]( [r,] T, member_seq )
 *
 * These macros expand to define a direct initialization constructor.  Their
 * purpose is to reduce boilerplate.
 *
 * SAKE_DIRECT_INIT_CTOR defines a constructor for class T which directly
 * initializes the members specified by the Boost.Preprocessor seq member_seq.
 *
 * Example:
 *
 * SAKE_DIRECT_INIT_CTOR( X, (( int, y )) (( float, z )) )
 *
 * expands to a constructor definition equivalent to
 *
 * X(int const y_, float const z_)
 *     y(y_), z(z_)
 * { }
 *
 * Members which are of a dependent type which are *not* prefixed with the
 * "typename" keyword *must* be prefixed with the "class" keyword.
 *
 * Example:
 *
 * template< class T >
 * struct X
 * {
 *     SAKE_DIRECT_INIT_CTOR( X, (( class T, y )), (( typename T::type, z )) )
 *     T y;
 *     typename T::type z;
 * };
 *
 * SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS defines a direct initialization
 * constructor via SAKE_DIRECT_INIT_CTOR and additionally declares the class's
 * members.  For this macro only, both T and member_seq may be prefixed with an
 * access keyword ("public", "private", or "protected") to indicate the access
 * of the constructor and members, respectively.
 *
 * Example:
 *
 * SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS( public X, private (( int, y )) )
 *
 * expands to a constructor definition and member declaration equivalent to
 *
 * public: explicit X(int const y_) : y(y_) { }
 * private: int y;
 * public:
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DIRECT_INIT_CTOR_HPP
#define SAKE_CORE_UTILITY_DIRECT_INIT_CTOR_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/deduce_r.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/preprocessor/keyword/access.hpp>
#include <sake/boost_ext/preprocessor/keyword/class.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>

#include <sake/core/utility/call_traits.hpp>

#define SAKE_DIRECT_INIT_CTOR_R( T, member_seq ) \
    SAKE_DIRECT_INIT_CTOR_R( BOOST_PP_DEDUCE_R(), T, member_seq )

#define SAKE_DIRECT_INIT_CTOR_R( r, T, member_seq ) \
    BOOST_PP_EXPR_IIF( BOOST_PP_EQUAL( BOOST_PP_SEQ_SIZE( member_seq ), 1 ), explicit ) \
    T ( BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_DIRECT_INIT_CTOR_comma_member_param_type, ~, member_seq ) ) \
        : BOOST_PP_SEQ_FOR_EACH_I_R( r, SAKE_DIRECT_INIT_CTOR_comma_init_member, ~, MemberSeq ) \
    { }

#define SAKE_DIRECT_INIT_CTOR_comma_member_param_type( r, data, i, elem ) \
    SAKE_DIRECT_INIT_CTOR_comma_member_param_type_impl( \
        i, \
        BOOST_PP_TUPLE_ELEM( 2, 0, elem ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, elem ) \
    )
#define SAKE_DIRECT_INIT_CTOR_comma_member_param_type_impl( i, type, name ) \
    BOOST_PP_COMMA_IF( i ) \
    BOOST_PP_EXPR_IIF( \
        BOOST_PP_BITOR( \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_TYPENAME( type ), \
            SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_CLASS( type ) \
        ), \
        typename \
    ) \
    ::sake::call_traits< SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( type ) >::param_type \
    BOOST_PP_CAT( _ ## i ## _, name )

#define SAKE_DIRECT_INIT_CTOR_comma_init_member( r, data, i, elem ) \
    SAKE_DIRECT_INIT_CTOR_comma_init_member_impl( i, BOOST_PP_TUPLE_ELEM( 2, 1, elem ) )
#define SAKE_DIRECT_INIT_CTOR_comma_init_member_impl( i, name ) \
    BOOST_PP_COMMA_IF( i ) name ( BOOST_PP_CAT( _ ## i ## _, name ) )

#define SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS( T, member_seq ) \
    SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS_R( BOOST_PP_DEDUCE_R(), T, member_seq )

#define SAKE_DIRECT_INIT_CTOR_DECLARE_MEMBERS_R( r, T, member_seq ) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_ACCESS_COLON( T ) \
    SAKE_DIRECT_INIT_CTOR_R( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_ACCESS( T ), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_ACCESS( member_seq ) \
    ) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_ACCESS_COLON( member_seq ) \
    BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        SAKE_DIRECT_INIT_CTOR_declare_member, \
        ~, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_ACCESS( member_seq ) \
    ) \
    SAKE_BOOST_EXT_PP_KEYWORD_GET_PREFIX_ACCESS_COLON( T )

#define SAKE_DIRECT_INIT_CTOR_declare_member( r, data, i, elem ) \
    SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_CLASS( BOOST_PP_TUPLE_ELEM( 2, 0, elem ) ) \
    BOOST_PP_TUPLE_ELEM( 2, 1, elem );

#endif // #ifndef SAKE_CORE_UTILITY_DIRECT_INIT_CTOR_HPP