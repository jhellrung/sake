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
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/tuple.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/emplacer/assign.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/emplacer/fwd.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>

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

    template< class Sequence >
    tuple(Sequence const &,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence const & >::value >::type* = 0)
    { }

    //tuple& operator=(const tuple & other)

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            tuple, Sequence const & >::value,
        tuple&
    >::type
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

#define Tn_seq( z, n, data ) \
    ( T ## n )
#define Tn_n_seq( z, n, data ) \
    (( T ## n )( _ ## n ))
#define Tn_n( z, n, data ) \
    T ## n _ ## n;
#define comma_i_emplacer_construct_Ti_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    BOOST_PP_CAT( _, i ) (sake::emplacer_construct< BOOST_PP_CAT( T, i ) >(elem))
#define _n_at_c_n_data( z, n, data ) \
    _ ## n (boost_ext::fusion::at_c<n>(data))
#define fwd_ref_Un_xn( z, n, data ) \
    SAKE_FWD_REF( U ## n ) x ## n
#define emplacer_assign_n_forward_Un_xn( z, n, data ) \
    sake::emplacer_assign(_ ## n, sake::forward< U ## n >(x ## n));
#define _n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    _ ## n = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/tuple.hpp>
#include BOOST_PP_ITERATE()

#undef Tn_seq
#undef Tn_n_seq
#undef Tn_n
#undef comma_i_emplacer_construct_Ti_elem
#undef _n_at_c_n_data
#undef fwd_ref_Un_xn
#undef emplacer_assign_n_forward_Un_xn
#undef _n_assign_at_c_n_forward_Sequence_s

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_FRAME_ITERATION( 1 )

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define T0N_seq    BOOST_PP_REPEAT( N, Tn_seq, ~ )
#define T0N_0N_seq BOOST_PP_REPEAT( N, Tn_n_seq, ~ )
#define T0N_0N     BOOST_PP_REPEAT( N, Tn_n, ~ )

#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )

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

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename tuple,
        T0N_0N_seq
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename tuple,
        ( default_constructor ) ( swap ) ( hash_value ),
        T0N_0N_seq
    )

    struct result_of
    {
        template< class This, std::size_t I >
        struct at_c
            : boost_ext::add_reference<
                  typename boost_ext::propagate_const<
                      This,
                      typename boost::mpl::at_c< values_type, I >::type
                  >::type
              >
        { };
        template< class This, class I >
        struct at
            : at_c< This, I::value >
        { };
    };

    template< std::size_t I >
    typename result_of::template at_c< tuple, I >::type
    at_c()
    { return private_::at_c_dispatch<I>::apply(*this); }
    template< std::size_t I >
    typename result_of::template at_c< tuple const, I >::type
    at_c() const
    { return private_::at_c_dispatch<I>::apply(*this); }

    template< class I >
    typename result_of::template at< tuple, I >::type
    at()
    { return at_c< I::value >(); }
    template< class I >
    typename result_of::template at< tuple const, I >::type
    at() const
    { return at_c< I::value >(); }

    T0N_0N

#if N == 1

    explicit tuple(sake::emplacer< void ( ) >)
    { }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit tuple(U0&& x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< tuple, U0 >::value
        >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(sake::forward< U0 >(x0)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit tuple(U0& x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< tuple, U0 >::value
        >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(x0))
    { }

    template< class U0 >
    explicit tuple(U0 const & x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< tuple, U0 >::value
        >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if N == 1

    // template< class U0, ... >
    // tuple(U0&& x0, ... );
#define SAKE_OVERLOAD_T U
#define SAKE_OVERLOAD_CONSTRUCTOR_NAME \
    tuple
#define SAKE_OVERLOAD_CONSTRUCTOR_INITIALIZATION_LIST( r, \
    n, U_tuple, x_tuple, forward_x_tuple ) \
    BOOST_PP_SEQ_FOR_EACH_I_R( r, \
        comma_i_emplacer_construct_Ti_elem, \
        ~, \
        BOOST_PP_TUPLE_TO_SEQ( n, forward_x_tuple ) \
    )
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         N
#if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY N
#else // #if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     N
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    N
#endif // #if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if N == 1

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    tuple(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence >::value >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, sake::forward< Sequence >(s) )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    tuple(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence& >::value >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, s )
    { }

    template< class Sequence >
    tuple(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence const & >::value >::type* = 0)
        : BOOST_PP_ENUM( N, _n_at_c_n_data, s )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class_U0N >
    void assign( BOOST_PP_ENUM( N, fwd_ref_Un_xn, ~ ) )
    { BOOST_PP_REPEAT( N, emplacer_assign_n_forward_Un_xn, ~ ) }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            tuple, SAKE_FWD_PARAM( Sequence ) >::value,
        tuple&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        BOOST_PP_REPEAT( N, _n_assign_at_c_n_forward_Sequence_s, ~ )
        return *this;
    }
};

} // namespace tuple_adl

#undef class_U0N

#undef T0N_seq
#undef T0N_0N_seq
#undef T0N_0N

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
