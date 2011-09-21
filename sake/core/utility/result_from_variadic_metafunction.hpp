/*******************************************************************************
 * sake/core/utility/result_from_variadic_metafunction.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_RESULT_FROM_VARIADIC_METAFUNCTION[_D,_Z,_DZ]( [d,] [z,] n_lower, n_upper, metafunction )
 *
 * This expands into a result struct definition (compatible with Boost.ResultOf)
 * in terms of a specified variadic metafunction.
 *
 * Example:
 *
 * namespace result_of
 * {
 * template< class T, class U = void >
 * struct f
 * { typedef ... type; };
 * } // namespace result_of
 *
 * namespace functional
 * {
 * struct f
 * {
 *     SAKE_RESULT_FROM_VARIADIC_METAFUNCTION( 1, 2, result_of::f )
 *     template< class T >
 *     typename result_of::f<T>::type // or "result< f ( T ) >::type"
 *     operator()(T x) const
 *     { ... }
 *     template< class T, class U >
 *     typename result_of::f<T,U>::type // or "result< f ( T, U ) >::type"
 *     operator()(T x, U y) const
 *     { ... }
 * };
 * } // namespace functional
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_RESULT_FROM_VARIADIC_METAFUNCTION_HPP
#define SAKE_CORE_UTILITY_RESULT_FROM_VARIADIC_METAFUNCTION_HPP

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/deduce_d.hpp>
#include <boost/preprocessor/repetition/deduce_z.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include <sake/core/utility/result_from_metafunction.hpp>

#define SAKE_RESULT_FROM_VARIADIC_METAFUNCTION( n_lower, n_upper, metafunction ) \
    SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_DZ( \
        BOOST_PP_DEDUCE_D(), BOOST_PP_DEDUCE_Z(), n_lower, n_upper, metafunction \
    )
#define SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_D( d, n_lower, n_upper, metafunction ) \
    SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_DZ( \
        d, BOOST_PP_DEDUCE_Z(), n_lower, n_upper, metafunction \
    )
#define SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_Z( z, n_lower, n_upper, metafunction ) \
    SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_DZ( \
        BOOST_PP_DEDUCE_D(), z, n_lower, n_upper, metafunction \
    )
#define SAKE_RESULT_FROM_VARIADIC_METAFUNCTION_DZ( d, z, n_lower, n_upper, metafunction ) \
    BOOST_PP_CAT( BOOST_PP_REPEAT_FROM_TO_D_, z ) ( \
        d, \
        n_lower, \
        BOOST_PP_INC( n_upper ), \
        SAKE_RESULT_FROM_METAFUNCTION_Z, \
        metafunction \
    )

#endif // #ifndef SAKE_CORE_UTILITY_RESULT_FROM_VARIADIC_METAFUNCTION_HPP
