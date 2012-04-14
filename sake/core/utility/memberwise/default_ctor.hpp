/*******************************************************************************
 * sake/core/utility/memberwise/default_ctor.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MEMBERWISE_DEFAULT_CTOR[_R]( [r,] T, type_seq )
 * #define SAKE_NATURAL_DEFAULT_CTOR_BODY()
 *
 * Expands to define a default constructor equivalent to member-wise default
 * construction. Additionally defines a has_default_constructor typedef based on
 * whether each type given in the Boost.PP Seq type_seq has a default
 * constructor (as determined by sake::has_default_constructor).
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP
#define SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/tuple/rem.hpp>

#include <sake/core/utility/has_default_constructor.hpp>

#define SAKE_MEMBERWISE_DEFAULT_CTOR( T, type_seq ) \
    SAKE_MEMBERWISE_DEFAULT_CTOR_R( BOOST_PP_DEDUCE_R(), T, type_seq )

#define SAKE_MEMBERWISE_DEFAULT_CTOR_R( r, T, type_seq ) \
    T() SAKE_NATURAL_DEFAULT_CTOR_BODY() \
    static bool const has_default_constructor_value = \
        BOOST_PP_CAT( \
            SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_, \
            SAKE_BOOST_EXT_PP_SEQ_IS_NIL( type_seq ) \
        ) ( r, type_seq ); \
    typedef ::boost::integral_constant< bool, has_default_constructor_value > has_default_constructor; \
    template< class, class > friend class ::sake::has_type_has_default_constructor;

#define SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_0( r, type_seq ) \
    BOOST_PP_CAT( \
        SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_0, \
        BOOST_PP_EQUAL( 1, BOOST_PP_SEQ_SIZE( type_seq ) ) \
    ) ( r, type_seq )
#define SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_00( r, type_seq ) \
    ::sake::boost_ext::mpl::BOOST_PP_CAT( and, BOOST_PP_SEQ_SIZE( type_seq ) )< \
        BOOST_PP_SEQ_FOR_EACH_I_R( r, \
            SAKE_MEMBERWISE_DEFAULT_CTOR_comma_has_default_constructor_elem, \
            ~, \
            type_seq \
        ) \
    >::value
#define SAKE_MEMBERWISE_DEFAULT_CTOR_comma_has_default_constructor_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) ::sake::has_default_constructor< elem >
#define SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_01( r, type_seq ) \
    ::sake::has_default_constructor< SAKE_BOOST_EXT_PP_TUPLE_REM1 type_seq >::value
#define SAKE_MEMBERWISE_DEFAULT_CTOR_has_default_constructor_value_1( r, type_seq ) \
    true

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_NATURAL_DEFAULT_CTOR_BODY() = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_NATURAL_DEFAULT_CTOR_BODY() { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP
