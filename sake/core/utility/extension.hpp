/*******************************************************************************
 * core/utility/extension.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_EXTENSION_UNARY_CLASS( Class )
 * SAKE_EXTENSION_CLASS( Class, Arity )
 * SAKE_EXTENSION_CLASS_N( Class, Arity, N )
 * SAKE_EXTENSION_FUNCTION( Function, TmplFnPrototype, CallForward, NamespaceSeq )
 * SAKE_EXTENSION_FUNCTION_N( Function, TmplFnPrototype, CallForward, NamespaceSeq, N )
 *
 * TODO: Documentation.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EXTENSION_HPP
#define SAKE_CORE_UTILITY_EXTENSION_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define SAKE_EXTENSION_UNARY_CLASS( Class ) \
namespace ext \
{ template< class T, class Enable = void > struct Class : no_ext::Class<T> { }; }

#define SAKE_EXTENSION_CLASS( Class, Arity ) \
namespace ext \
{ \
    template< BOOST_PP_ENUM_PARAMS( Arity, class T ), class Enable = void > \
    struct Class : no_ext::Class< BOOST_PP_ENUM_PARAMS( Arity, T ) > { }; \
}



#define SAKE_EXTENSION_CLASS_N( Class, Arity, N ) \
namespace ext \
{ \
    BOOST_PP_REPEAT( N, SAKE_EXTENSION_declare_Trait_n, ( Class, Arity ) ) \
    BOOST_PP_REPEAT_FROM_TO( 0, BOOST_PP_DEC( N ), SAKE_EXTENSION_define_Trait_n, ( Class, Arity ) ) \
    template< BOOST_PP_ENUM_PARAMS( Arity, class T ), class Enable > \
    struct BOOST_PP_CAT( Class, BOOST_PP_DEC( N ) ) : no_ext::Class< BOOST_PP_ENUM_PARAMS( Arity, T ) > { }; \
}

#define SAKE_EXTENSION_declare_Trait_n( z, n, data ) \
    SAKE_EXTENSION_declare_Trait_n_helper( \
        BOOST_PP_TUPLE_ELEM( 2, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, data ), \
        n \
    )
#define SAKE_EXTENSION_declare_Trait_n_helper( Class, Arity, n ) \
    template< BOOST_PP_ENUM_PARAMS( Arity, class T ), class Enable = void > \
    struct BOOST_PP_CAT( Class, n );

#define SAKE_EXTENSION_define_Trait_n( z, n, data ) \
    SAKE_EXTENSION_define_Trait_n_helper( \
        BOOST_PP_TUPLE_ELEM( 2, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, data ), \
        n \
    )
#define SAKE_EXTENSION_define_Trait_n_helper( Class, Arity, n ) \
    template< BOOST_PP_ENUM_PARAMS( Arity, class T ), class Enable > \
    struct BOOST_PP_CAT( Class, n ) : \
        BOOST_PP_CAT( Class, BOOST_PP_INC( n ) )< BOOST_PP_ENUM_PARAMS( Arity, T ) > \
    { };



#define SAKE_EXTENSION_FUNCTION( Function, TmplFnPrototype, CallForward, NamespaceSeq ) \
namespace BOOST_PP_SEQ_CAT( ( sake_ ) ( Function ) ( _private ) ) \
{ \
TmplFnPrototype( BOOST_PP_CAT( Function, _impl ) ) \
{ \
    using namespace BOOST_PP_SEQ_FOR_EACH( SAKE_EXTENSION_scope, ~, NamespaceSeq ) ::no_adl; \
    return CallForward( Function ); \
} \
}

#define SAKE_EXTENSION_scope( r, data, elem ) :: elem



#define SAKE_EXTENSION_FUNCTION_N( Function, TmplFnPrototype, CallForward, NamespaceSeq, N ) \
namespace BOOST_PP_SEQ_CAT( ( sake_ ) ( Function ) ( _private ) ) { \
BOOST_PP_REPEAT( N, SAKE_EXTENSION_declare_impl_n, TmplFnPrototype ) \
} \
BOOST_PP_SEQ_FOR_EACH( SAKE_EXTENSION_open_namespace, ~, NamespaceSeq ) \
namespace BOOST_PP_CAT( Function, _no_adl ) { \
BOOST_PP_REPEAT_FROM_TO( \
    0, BOOST_PP_DEC( N ), \
    SAKE_EXTENSION_define_Function_n, \
    ( Function, TmplFnPrototype, CallForward ) \
) \
TmplFnPrototype( BOOST_PP_CAT( Function, BOOST_PP_DEC( N ) ) ) \
{ return CallForward( BOOST_PP_SEQ_FOR_EACH( SAKE_EXTENSION_scope, ~, NamespaceSeq ) :: BOOST_PP_CAT( Function, _no_adl ) ::Function ); } \
} \
BOOST_PP_SEQ_FOR_EACH( SAKE_EXTENSION_close_namespace, ~, NamespaceSeq ) \
namespace BOOST_PP_SEQ_CAT( ( sake_ ) ( Function ) ( _private ) ) { \
BOOST_PP_REPEAT( \
    N, \
    SAKE_EXTENSION_define_impl_n, \
    ( Function, TmplFnPrototype, CallForward, NamespaceSeq ) \
) \
}

#define SAKE_EXTENSION_open_namespace( r, data, elem ) namespace elem {
#define SAKE_EXTENSION_close_namespace( r, data, elem ) }

#define SAKE_EXTENSION_declare_impl_n( z, n, data ) data( BOOST_PP_CAT( impl, n ) );

#define SAKE_EXTENSION_define_Function_n( z, n, data ) \
    SAKE_EXTENSION_define_Function_n_helper( \
        BOOST_PP_TUPLE_ELEM( 3, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 3, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 3, 2, data ), \
        n \
    )
#define SAKE_EXTENSION_define_Function_n_helper( Function, TmplFnPrototype, CallForward, n ) \
    TmplFnPrototype( BOOST_PP_CAT( Function, n ) ) \
    { \
        return CallForward( \
            :: BOOST_PP_SEQ_CAT( ( sake_ ) ( Function ) ( _private ) ) :: \
            BOOST_PP_CAT( impl, BOOST_PP_INC( n ) ) \
        ); \
    }

#define SAKE_EXTENSION_define_impl_n( z, n, data ) \
    SAKE_EXTENSION_define_impl_n_helper( \
        BOOST_PP_TUPLE_ELEM( 4, 0, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 1, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 2, data ), \
        BOOST_PP_TUPLE_ELEM( 4, 3, data ), \
        n \
    )
#define SAKE_EXTENSION_define_impl_n_helper( Function, TmplFnPrototype, CallForward, NamespaceSeq, n ) \
    TmplFnPrototype( BOOST_PP_CAT( impl, n ) ) \
    { \
        using namespace BOOST_PP_SEQ_FOR_EACH( SAKE_EXTENSION_scope, ~, NamespaceSeq ) :: BOOST_PP_CAT( Function, _no_adl ); \
        return CallForward( BOOST_PP_CAT( Function, n ) ); \
    }

#endif // #ifndef SAKE_CORE_UTILITY_EXTENSION_HPP
