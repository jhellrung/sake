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
#include <boost/preprocessor/repetition/deduce_r.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/preprocessor/seq/is_nil.hpp>
#include <sake/boost_ext/preprocessor/seq/to_mpl_vector.hpp>

#include <sake/core/utility/define_natural/private/member_name.hpp>
#include <sake/core/utility/has_default_constructor.hpp>

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR( T, base_seq, member_seq ) \
    SAKE_DEFINE_NATURAL_DEFAULT_CTOR_R( BOOST_PP_DEDUCE_R(), T, base_seq, member_seq )

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_R( r, T, base_seq, member_seq ) \
    T() SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body \
    static bool const has_default_constructor = \
    BOOST_PP_SEQ_CAT( \
        ( SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_ ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( base_seq ) ) \
        ( SAKE_BOOST_EXT_PP_SEQ_IS_NIL( member_seq ) ) \
    ) ( r, base_seq, member_seq ) ;

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_00( r, base_seq, member_seq ) \
    ::sake::boost_ext::mpl::all< \
        SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( \
            base_seq \
            BOOST_PP_SEQ_FOR_EACH_R( r, \
                SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_name, \
                ~, \
                member_seq \
            ) \
        ), \
        ::boost::mpl::quote1< ::sake::has_default_constructor > \
    >::type::value

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_01( r, base_seq, member_seq ) \
    ::sake::boost_ext::mpl::all< \
        SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( base_seq ), \
        ::boost::mpl::quote1< ::sake::has_default_constructor > \
    >::type::value

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_10( r, base_seq, member_seq ) \
    ::sake::boost_ext::mpl::all< \
        SAKE_BOOST_EXT_PP_SEQ_TO_MPL_VECTOR( \
            BOOST_PP_SEQ_FOR_EACH_R( r, \
                SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_name, \
                ~, \
                member_seq \
            ) \
        ), \
        ::boost::mpl::quote1< ::sake::has_default_constructor > \
    >::type::value

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_has_default_constructor_value_11( r, base_seq, member_seq ) \
    true

#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_member_name( r, data, elem ) \
    ( SAKE_DEFINE_NATURAL_member_name( elem ) )

#ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body = default;
#else // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS
#define SAKE_DEFINE_NATURAL_DEFAULT_CTOR_body { }
#endif // #ifndef BOOST_NO_DEFAULTED_FUNCTIONS

#endif // #ifndef SAKE_CORE_UTILITY_DEFINE_NATURAL_DEFAULT_CTOR_HPP
