/*******************************************************************************
 * sake/core/data_structures/tuple/tuple.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct tuple< T0, ... >
 *
 * tuple_adl::swap(tuple< T0, ... >& x, tuple< U0, ... >& y) -> void
 * tuple_adl::hash_value(tuple< T0, ... > const & x) -> std::size_t
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TUPLE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_TUPLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/tuple.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/mpl/vector.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/data_structures/tuple/private/operator_assign_enable.hpp>
#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>

#define SAKE_TUPLE_INCLUDE_HEADERS
#include <sake/core/data_structures/tuple/private/at.ipp>
#include <sake/core/data_structures/tuple/private/value_constructor.ipp>
#include <sake/core/data_structures/tuple/private/sequence_constructor.ipp>
#include <sake/core/data_structures/tuple/private/operator_assign.ipp>
#include <sake/core/data_structures/tuple/private/assign.ipp>
#undef SAKE_TUPLE_INCLUDE_HEADERS

namespace sake
{

/*******************************************************************************
 * struct tuple<>
 ******************************************************************************/

namespace tuple_adl
{

template<>
struct tuple<>
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector<> values_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::vector0<> values_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    static std::size_t const static_size = 0;

    SAKE_MEMBERWISE_MEM_FUN(
        tuple,
        ( default_constructor ) ( swap ) ( hash_value ),
        BOOST_PP_SEQ_NIL
    )

private:
    template< class Sequence >
    struct sequence_constructor_enabler
        : sake::tuple_private::sequence_constructor_enabler< tuple, Sequence >
    { };
public:
    template< class Sequence >
    tuple(Sequence const &,
        typename sequence_constructor_enabler< Sequence const & >::type* = 0)
    { }

    //tuple& operator=(const tuple & other)

private:
    template< class Sequence >
    struct operator_assign_enabler
        : sake::tuple_private::operator_assign_enabler< tuple, Sequence >
    { };
public:
    template< class Sequence >
    typename operator_assign_enabler< Sequence const & >::type // -> tuple&
    operator=(Sequence const &)
    { return *this; }
};

} // namespace tuple_adl

/*******************************************************************************
 * struct tuple< T0, ... >
 ******************************************************************************/

namespace tuple_adl
{

namespace private_
{

template< std::size_t N >
struct at_c_dispatch;

} // namespace private_

} // namespace tuple_adl

#define Tn_n_seq( z, n, data ) \
    (( T ## n )( _ ## n ))
#define Tn_n( z, n, data ) \
    T ## n _ ## n;
#define typedef_remove_cv_Tn_nocvn_type( z, n, data ) \
    typedef typename boost::remove_cv< T ## n >::type nocv ## n ## _type;
#define comma_i_emplacer_constructible_nocvi_type_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) BOOST_PP_CAT( _, i ) ( \
        sake::emplacer_constructible< \
            BOOST_PP_SEQ_CAT( ( nocv ) ( i ) ( _type ) ) \
        >(elem) \
    )
#define _n_at_c_n_data( z, n, data ) \
    _ ## n (boost_ext::fusion::at_c<n>(data))
#define _n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    _ ## n = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));
#define fwd_ref_Un_xn( z, n, data ) \
    SAKE_FWD_REF( U ## n ) x ## n
#define fwd2_ref_Un_xn( z, n, data ) \
    SAKE_FWD2_REF( U ## n ) x ## n
#define forward_Un_xn( z, n, data ) \
    sake::forward< U ## n >(x ## n)
#define _n_assign_forward_Un_xn( z, n, data ) \
    _ ## n = sake::forward< U ## n >(x ## n);

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/tuple.hpp>
#include BOOST_PP_ITERATE()

#undef Tn_n_seq
#undef Tn_n
#undef typedef_remove_cv_Tn_nocvn_type
#undef comma_i_emplacer_constructible_nocvi_type_elem
#undef _n_at_c_n_data
#undef _n_assign_at_c_n_forward_Sequence_s
#undef fwd_ref_Un_xn
#undef fwd2_ref_Un_xn
#undef forward_Un_xn
#undef _n_assign_forward_Un_xn

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_FRAME_ITERATION( 1 )

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define T0N_0N_seq BOOST_PP_REPEAT( N, Tn_n_seq, ~ )

#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define U0N       BOOST_PP_ENUM_PARAMS( N, U )

namespace tuple_adl
{

namespace private_
{

template<>
struct at_c_dispatch< BOOST_PP_DEC( N ) >
{
    template< class T >
    static typename T::result_of::template at_c< T, BOOST_PP_DEC( N ) >::type
    apply(T& x)
    { return BOOST_PP_CAT( x._, BOOST_PP_DEC( N ) ); }
};

} // namespace private_

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct tuple
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct tuple< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0N > values_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > values_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    static std::size_t const static_size = N;

    BOOST_PP_REPEAT( N, Tn_n, ~ )

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename tuple,
        T0N_0N_seq
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename tuple,
        ( default_constructor ) ( swap ) ( hash_value ),
        T0N_0N_seq
    )

private:
    BOOST_PP_REPEAT( N, typedef_remove_cv_Tn_nocvn_type, ~ )
public:

#if 0 // for exposition purposes only
    struct result_of
    {
        template< class This, std::size_t I > struct at_c;
        template< class This, class I > struct at;
    };
    template< std::size_t I >
    typename result_of::template at_c< tuple [const], I >::type
    at_c() [const];
    template< class I >
    typename result_of::template at< tuple [const], I >::type
    at() [const];

    template< class... U >
    tuple(U&&... x,
        typename value_constructor_enabler< U... >::type* = 0);

    template< class Sequence >
    tuple(Sequence&& s,
        typename sequence_constructor_enabler< Sequence >::type* = 0);

    template< class Sequence >
    typename operator_assign_enabler< Sequence >::type // -> tuple&
    operator=(Sequence&& s);

    template< class... U >
    typename assign_enabler< U... >::type // -> void
    assign(U&&... x);
#endif
#define SAKE_TUPLE_DEFINE_MEMBERS
#include <sake/core/data_structures/tuple/private/at.ipp>
#include <sake/core/data_structures/tuple/private/value_constructor.ipp>
#include <sake/core/data_structures/tuple/private/sequence_constructor.ipp>
#include <sake/core/data_structures/tuple/private/operator_assign.ipp>
#include <sake/core/data_structures/tuple/private/assign.ipp>
#undef SAKE_TUPLE_DEFINE_MEMBERS

};

} // namespace tuple_adl

#undef class_U0N

#undef T0N_0N_seq

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
