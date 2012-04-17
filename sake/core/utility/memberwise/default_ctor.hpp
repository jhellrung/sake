/*******************************************************************************
 * sake/core/utility/memberwise/default_ctor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DEFAULT_CTOR[_R]( [r,] T, member_seq )
 * #define SAKE_MEMBERWISE_DEFAULT_CTOR_BODY()
 *
 * Expands to define a default constructor equivalent to member-wise default
 * construction. Additionally defines a has_default_constructor_tag typedef
 * based on whether each member specified in member_seq has a default
 * constructor (as determined by sake::has_default_constructor).
 *
 * member_seq should be a Boost.PP sequence of base class specifiers followed by
 * member variable specifiers. A base class specifier is a Boost.PP 1-sequence
 * simply consisting of the base class; a member variable specifier is a
 * Boost.PP 2-sequence of the form ( type )( name ).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_MEMBERWISE_DEFAULT_CTOR_HPP
#define SAKE_CORE_UTILITY_MEMBERWISE_DEFAULT_CTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/preprocessor/keyword/typename.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>

#include <sake/core/utility/has_default_constructor.hpp>

#define SAKE_MEMBERWISE_DEFAULT_CTOR( T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CTOR_impl( BOOST_PP_DEDUCE_R(), \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )
#define SAKE_MEMBERWISE_DEFAULT_CTOR_R( r, T, member_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CTOR_impl( r, \
        SAKE_BOOST_EXT_PP_KEYWORD_REMOVE_PREFIX_TYPENAME( T ), \
        member_seq \
    )

#define SAKE_MEMBERWISE_DEFAULT_CTOR_impl( r, T, member_seq ) \
    T() SAKE_MEMBERWISE_DEFAULT_CTOR_BODY() \
    static bool const has_default_constructor_tag_value = \
        BOOST_PP_CAT( \
            SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_, \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) \
        ) ( r, member_seq ); \
    typedef ::boost::integral_constant< bool, has_default_constructor_tag_value > has_default_constructor_tag; \
    template< class, class > friend class ::sake::has_type_has_default_constructor_tag;

#define SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_0( r, member_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_0, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( member_seq ) ) \
    ) ( r, member_seq )
#define SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_00( r, member_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( and, BOOST_PP_SEQ_SIZE( member_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_DEFAULT_CTOR_comma_has_default_constructor, ~, member_seq ) \
    >::value
#define SAKE_MEMBERWISE_DEFAULT_CTOR_comma_has_default_constructor( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::has_default_constructor< BOOST_PP_SEQ_HEAD( elem ) >
#define SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_01( r, member_seq ) \
    ::sake::has_default_constructor< BOOST_PP_SEQ_HEAD( BOOST_PP_SEQ_HEAD( member_seq ) ) >::value

#define SAKE_MEMBERWISE_DEFAULT_CTOR_any_has_default_constructor_1( r, member_seq ) \
    true

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DEFAULT_CTOR_BODY() = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_MEMBERWISE_DEFAULT_CTOR_BODY() { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_MEMBERWISE_DEFAULT_CTOR_HPP
