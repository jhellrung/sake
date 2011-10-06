/*******************************************************************************
 * sake/core/utility/result_from_metafunction.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_RESULT_FROM_METAFUNCTION[_DZ]( [d,] [z,] metafunction, n )
 *
 * This expands into a result struct definition (compatible with Boost.ResultOf)
 * in terms of a specified metafunction.
 *
 * Example:
 *
 * namespace result_of
 * {
 * template< class T >
 * struct f
 * { typedef ... type; };
 * } // namespace result_of
 *
 * namespace functional
 * {
 * struct f
 * {
 *     SAKE_RESULT_FROM_METAFUNCTION( result_of::f, 1 )
 *     template< class T >
 *     typename result_of::f<T>::type // or "result< f ( T ) >::type"
 *     operator()(T x) const
 *     { ... }
 * };
 * } // namespace functional
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP
#define SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/deduce_d.hpp>
#include <boost/preprocessor/repetition/deduce_z.hpp>
#include <boost/preprocessor/detail/is_binary.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define SAKE_RESULT_FROM_METAFUNCTION( metafunction, n ) \
    SAKE_RESULT_FROM_METAFUNCTION_DZ( BOOST_PP_DEDUCE_D(), BOOST_PP_DEDUCE_Z(), metafunction, n )
#define SAKE_RESULT_FROM_METAFUNCTION_D( d, metafunction, n ) \
    SAKE_RESULT_FROM_METAFUNCTION_DZ( d, BOOST_PP_DEDUCE_Z(), metafunction, n )
#define SAKE_RESULT_FROM_METAFUNCTION_Z( z, metafunction, n ) \
    SAKE_RESULT_FROM_METAFUNCTION_DZ( BOOST_PP_DEDUCE_D(), z, metafunction, n )

#define SAKE_RESULT_FROM_METAFUNCTION_DZ( d, z, metafunction, n ) \
    template< class > struct result; \
    BOOST_PP_CAT( SAKE_RESULT_FROM_METAFUNCTION_dispatch, BOOST_PP_IS_BINARY( n ) ) ( d, z, metafunction, n )
#define SAKE_RESULT_FROM_METAFUNCTION_dispatch0( d, z, metafunction, n ) \
    SAKE_RESULT_FROM_METAFUNCTION_result( z, n, metafunction )
#define SAKE_RESULT_FROM_METAFUNCTION_dispatch1( d, z, metafunction, n ) \
    BOOST_PP_CAT( BOOST_PP_REPEAT_FROM_TO_D_, z ) ( \
        d, \
        BOOST_PP_TUPLE_ELEM( 2, 0, n ), \
        BOOST_PP_INC( BOOST_PP_TUPLE_ELEM( 2, 1, n ) ), \
        SAKE_RESULT_FROM_METAFUNCTION_result, \
        metafunction \
    )

#define SAKE_RESULT_FROM_METAFUNCTION_result( z, n, data ) \
    template< class This, BOOST_PP_ENUM_PARAMS_Z( z, n, class T ) > \
    struct result< This ( BOOST_PP_ENUM_PARAMS_Z( z, n, T ) ) > \
        : data< BOOST_PP_ENUM_PARAMS_Z( z, n, T ) > \
    { };

#endif // #ifndef SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP
