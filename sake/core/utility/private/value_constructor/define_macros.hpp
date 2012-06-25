/*******************************************************************************
 * sake/core/utility/private/value_constructor/define_macros.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_PRIVATE_VALUE_CONSTRUCTOR_DEFINE_MACROS_HPP
#define SAKE_CORE_UTILITY_PRIVATE_VALUE_CONSTRUCTOR_DEFINE_MACROS_HPP

#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/utility/value_constructor_enable.hpp>

#define SAKE_VALUE_CONSTRUCTOR_arity \
    BOOST_PP_SEQ_SIZE( SAKE_VALUE_CONSTRUCTOR_TYPES )

#define SAKE_VALUE_CONSTRUCTOR_class_T0N \
    BOOST_PP_ENUM_PARAMS( SAKE_VALUE_CONSTRUCTOR_arity, class _SakeValueConstructorT )
#define SAKE_VALUE_CONSTRUCTOR_T0N \
    BOOST_PP_ENUM_PARAMS( SAKE_VALUE_CONSTRUCTOR_arity, _SakeValueConstructorT )

#define SAKE_VALUE_CONSTRUCTOR_emplacer_T0N \
    BOOST_PP_ENUM_BINARY_PARAMS( \
        SAKE_VALUE_CONSTRUCTOR_arity, \
        ::sake::emplacer< _SakeValueConstructorT, ( ) > BOOST_PP_INTERCEPT \
    )

#define SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enable \
    BOOST_PP_SEQ_CAT( \
        ( _sake_value_constructor_ ) \
        ( SAKE_VALUE_CONSTRUCTOR_arity ) \
        ( _enable ) \
    )

#define SAKE_VALUE_CONSTRUCTOR_sake_value_constructor_N_enabler \
    BOOST_PP_SEQ_CAT( \
        ( _sake_value_constructor_ ) \
        ( SAKE_VALUE_CONSTRUCTOR_arity ) \
        ( _enabler ) \
    )

#define SAKE_VALUE_CONSTRUCTOR_type0 \
    BOOST_PP_SEQ_ELEM( 0, SAKE_VALUE_CONSTRUCTOR_TYPES )
#define SAKE_VALUE_CONSTRUCTOR_type1 \
    BOOST_PP_SEQ_ELEM( 1, SAKE_VALUE_CONSTRUCTOR_TYPES )

#define SAKE_VALUE_CONSTRUCTOR_value_constructor_enable_typen_Tn( z, n, data ) \
    ::sake::value_constructor_enable< \
        BOOST_PP_SEQ_ELEM( n, SAKE_VALUE_CONSTRUCTOR_TYPES ), \
        _SakeValueConstructorT ## n \
    >

#ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_VALUE_CONSTRUCTOR_forward_Tn_xn( z, n, data ) \
    ::sake::forward< _SakeValueConstructorT ## n >( \
        _sake_value_constructor_x ## n)

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_UTILITY_PRIVATE_VALUE_CONSTRUCTOR_DEFINE_MACROS_HPP
