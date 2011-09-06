/*******************************************************************************
 * core/utility/result_from_metafunction.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_RESULT_FROM_METAFUNCTION[_Z]( [z,] N, Metafunction)
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
 *     SAKE_RESULT_FROM_METAFUNCTION( 1, result_of::f )
 *     template< class T >
 *     typename result_of::f<T>::type // or "result< f ( T ) >::type"
 *     operator()(T x) const
 *     { ... }
 * };
 * } // namespace functional
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP
#define SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP

#include <boost/preprocessor/repetition/deduce_z.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define SAKE_RESULT_FROM_METAFUNCTION( N, Metafunction ) \
    SAKE_RESULT_FROM_METAFUNCTION_Z( BOOST_PP_DEDUCE_Z(), N, Metafunction )
#define SAKE_RESULT_FROM_METAFUNCTION_Z( z, N, Metafunction ) \
    template<class> struct result; \
    template< class This, BOOST_PP_ENUM_PARAMS_Z( z, N, class T ) > \
    struct result< This ( BOOST_PP_ENUM_PARAMS_Z( z, N, T ) ) > \
        : Metafunction< BOOST_PP_ENUM_PARAMS_Z( z, N, T ) > \
    { };

#endif // #ifndef SAKE_CORE_UTILITY_RESULT_FROM_METAFUNCTION_HPP
