/*******************************************************************************
 * sake/core/utility/define_natural/default_ctor.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_DEFINE_NATURAL_DEFAULT_CTOR[_R]( [r,] T, base_seq, member_seq )
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/facilities/apply.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/preprocessor/logical/compl.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/to_mpl_vector.hpp>

#include <sake/core/utility/has_default_constructor.hpp>

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR( T, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_R( r, T, base_seq, member_seq ) \
    T() SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_dispatch, \
        SAKE_DEFINE_NATURAL_DEFAULT_CTOR_all_is_binary( r, member_seq ) \
    ) ( r, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_all_is_binary( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_DEFAULT_CTOR_all_is_binary_, \
        SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
    ) ( r, member_seq )
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_all_is_binary_0( r, member_seq ) \
    BOOST_PP_IS_EMPTY( BOOST_PP_SEQ_FOR_EACH_R( r, \
        SAKE_DEFINE_NATURAL_DEFAULT_CTOR_expr_iif_not_is_binary, ~, member_seq ) )
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_expr_iif_not_is_binary( r, data, elem ) \
    BOOST_PP_EXPR_IIF( BOOST_PP_COMPL( BOOST_PP_IS_BINARY( elem ) ), BOOST_PP_NIL )
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_all_is_binary_1( r, member_seq ) 1

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_dispatch0( r, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_dispatch1( r, base_seq, member_seq ) \
    static bool const has_default_constructor_value = \
        BOOST_PP_SEQ_CAT( \
            ( SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_ ) \
            ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ) \
            ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) ) \
        ) ( r, base_seq, member_seq ); \
    typedef ::boost::integral_constant< bool, has_default_constructor_value > has_default_constructor; \
    template< class, class > friend class ::sake::has_type_has_default_constructor;

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_00( r, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl( \
        base_seq BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_type, ~, member_seq ) )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_01( r, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl( \
        base_seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_10( r, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl( \
        BOOST_PP_SEQ_FOR_EACH_R( r, SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_type, ~, member_seq ) )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_11( r, base_seq, member_seq ) \
    true

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl( seq ) \
    BOOST_PP_CAT( \
        SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl_, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( seq ) ) \
    ) ( seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl_0( seq ) \
    ::sake::boost_ext::mpl::all< \
        SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( seq ), \
        ::boost::mpl::quote1< ::sake::has_default_constructor > \
    >::type::value
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_impl_1( seq ) \
    ::sake::has_default_constructor< BOOST_PP_APPLY( seq ) >::value

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_type( r, data, elem ) \
    ( BOOST_PP_TUPLE_ELEM( 2, 0, elem ) )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP
