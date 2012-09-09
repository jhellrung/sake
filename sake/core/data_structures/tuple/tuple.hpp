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
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/tuple.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/mpl/vector.hpp>

#include <sake/core/data_structures/tuple/fwd.hpp>
#include <sake/core/data_structures/tuple/private/base.hpp>
#include <sake/core/data_structures/tuple/private/operator_assign_enable.hpp>
#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/using_typedef.hpp>
#include <sake/core/utility/value_constructor.hpp>

#define SAKE_TUPLE_INCLUDE_HEADERS
#include <sake/core/data_structures/tuple/private/at.ipp>
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
    typedef boost_ext::mpl::vector<> value_types;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::vector0<> value_types;
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

#define using_base_n( z, n, data ) \
    using base_::_ ## n;
#define SAKE_USING_TYPEDEF_typename_base_nocvn_type( z, n, data ) \
    SAKE_USING_TYPEDEF( typename base_, nocv ## n ## _type );
#define Tn_seq( z, n, data ) \
    ( T ## n )
#define nocvn_type( z, n, data ) \
    nocv ## n ## _type
#define at_c_n_data( z, n, data ) \
    boost_ext::fusion::at_c<n>(data)
#define _n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    _ ## n = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));
#define fwd_ref_Un_xn( z, n, data ) \
    SAKE_FWD_REF( U ## n ) x ## n
#define fwd2_param_Un( z, n, data ) \
    SAKE_FWD2_PARAM( U ## n )
#define fwd2_ref_Un_xn( z, n, data ) \
    SAKE_FWD2_REF( U ## n ) x ## n
#define forward_Un_xn( z, n, data ) \
    sake::forward< U ## n >(x ## n)
#define _n_assign_forward_Un_xn( z, n, data ) \
    _ ## n = sake::forward< U ## n >(x ## n);

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/tuple.hpp>
#include BOOST_PP_ITERATE()

#undef using_base_n
#undef SAKE_USING_TYPEDEF_typename_base_nocvn_type
#undef Tn_seq
#undef nocvn_type
#undef at_c_n_data
#undef _n_assign_at_c_n_forward_Sequence_s
#undef fwd_ref_Un_xn
#undef fwd2_param_Un
#undef fwd2_ref_Un_xn
#undef forward_Un_xn
#undef _n_assign_forward_Un_xn

} // namespace tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_FRAME_ITERATION( 1 )

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define U0N       BOOST_PP_ENUM_PARAMS( N, U )

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
class tuple
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
class tuple< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
#if N != 1
    : public private_::base< T0N >
#endif // #if N != 1
{
#if N != 1
    typedef private_::base< T0N > base_;
#endif // #if N != 1
public:
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0N > value_types;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > value_types;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    static std::size_t const static_size = N;

#if N == 1
    T0 _0;
#define member_seq (( T0 )( _0 ))
#else // #if N == 1
    BOOST_PP_REPEAT( N, using_base_n, ~ )
#define member_seq (( base_ ))
#endif // #if N == 1

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename tuple,
        member_seq
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename tuple,
        ( default_constructor ) ( swap ) ( hash_value ),
        member_seq
    )

#undef member_seq

private:
#if N == 1
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
#else // #if N == 1
    BOOST_PP_REPEAT( N, SAKE_USING_TYPEDEF_typename_base_nocvn_type, ~ )
#endif // #if N == 1
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector<
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )<
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
        BOOST_PP_ENUM( N, nocvn_type, ~ ) > nocv_types;
public:

    // template< class... U >
    // tuple(U&&... x,
    //     typename value_constructor_enabler< U... >::type* = 0);
#define SAKE_VALUE_CONSTRUCTOR_CLASS_NAME tuple
#define SAKE_VALUE_CONSTRUCTOR_TYPES      BOOST_PP_REPEAT( N, Tn_seq, ~ )
#if N == 1
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    _0
#else // #if N == 1
#define SAKE_VALUE_CONSTRUCTOR_FORWARD    base_
#endif // #if N == 1
#if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#define SAKE_VALUE_CONSTRUCTOR_PERFECT_FORWARDING
#endif // #if N <= SAKE_TUPLE_PERFECT_MAX_ARITY
#include SAKE_VALUE_CONSTRUCTOR_GENERATE()

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
#include <sake/core/data_structures/tuple/private/sequence_constructor.ipp>
#include <sake/core/data_structures/tuple/private/operator_assign.ipp>
#include <sake/core/data_structures/tuple/private/assign.ipp>
#undef SAKE_TUPLE_DEFINE_MEMBERS

};

#undef class_U0N
#undef U0N

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
