/*******************************************************************************
 * sake/core/data_structures/tuple/private/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_BASE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_BASE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>

namespace sake
{

namespace tuple_adl
{

namespace private_
{

struct sequence_rv_sink_default_tag { };

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_BINARY_PARAMS( SAKE_TUPLE_MAX_SIZE, class T, = void BOOST_PP_INTERCEPT ) >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct base;

#define Tn_n_seq( z, n, data ) \
    (( T ## n )( _ ## n ))
#define Tn_n( z, n, data ) \
    T ## n _ ## n;
#define typedef_remove_cv_Tn_nocvn_type( z, n, data ) \
    typedef typename boost::remove_cv< T ## n >::type nocv ## n ## _type;
#define SAKE_FWD2_REF_Un_xn( z, n, data ) \
    SAKE_FWD2_REF( U ## n ) x ## n
#define _n_emplacer_constructible_nocvn_type_forward_Un_xn( z, n, data ) \
    _ ## n (sake::emplacer_constructible< nocv ## n ## _type >(sake::forward< U ## n >(x ## n)))
#define _n_data_at_n( z, n, data ) \
    _ ## n (data[boost::integral_constant< std::size_t, n >()])

#define BOOST_PP_ITERATION_LIMITS ( 2, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/private/base.hpp>
#include BOOST_PP_ITERATE()

#undef Tn_n_seq
#undef Tn_n
#undef typedef_remove_cv_Tn_nocvn_type
#undef SAKE_FWD2_REF_Un_xn
#undef _n_emplacer_constructible_nocvn_type_forward_Un_xn
#undef _n_data_at_n

} // namespace private_

} // namespace tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_PRIVATE_BASE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define T0N_0N_seq BOOST_PP_REPEAT( N, Tn_n_seq, ~ )

#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define U0N       BOOST_PP_ENUM_PARAMS( N, U )

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct base
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct base< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
{
    BOOST_PP_REPEAT( N, Tn_n, ~ )

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename base,
        T0N_0N_seq
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base,
        ( default_constructor ) ( swap ) ( hash_value ),
        T0N_0N_seq
    )

protected:
    BOOST_PP_REPEAT( N, typedef_remove_cv_Tn_nocvn_type, ~ )

#if N == 2

    template< class V0, class V1 >
    base(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >)
    { }

    template< class U0, class V1 >
    base(SAKE_FWD2_REF( U0 ) x0, sake::emplacer< V1 ( ) >)
        : _0(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0)))
    { }

    template< class V0, class U1 >
    base(sake::emplacer< V0 ( ) >, SAKE_FWD2_REF( U1 ) x1)
        : _1(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

#endif // #if N == 2

    template< class_U0N >
    base(BOOST_PP_ENUM( N, SAKE_FWD2_REF_Un_xn, ~ ))
        : BOOST_PP_ENUM( N, _n_emplacer_constructible_nocvn_type_forward_Un_xn, ~ )
    { }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template< class SequenceRVSinkDefault >
    base(private_::sequence_rv_sink_default_tag, SequenceRVSinkDefault & s)
        : BOOST_PP_ENUM( N, _n_data_at_n, s )
    { }
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

};

#undef class_U0N
#undef U0N

#undef T0N_0N_seq

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
