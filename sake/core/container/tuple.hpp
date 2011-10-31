/*******************************************************************************
 * sake/core/container/tuple.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct tuple< T0, ... >
 * make_tuple(T0&& x0, ... ) -> result_of::make_tuple< T0, ... >::type
 *
 * swap(tuple< T0, ... >& x, tuple< U0, ... >& y) -> void
 * hash_value(tuple< T0, ... > const & x) -> std::size_t
 *
 * operator==(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator!=(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator<(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * compare(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_CONTAINER_TUPLE_HPP
#define SAKE_CORE_CONTAINER_TUPLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/tuple.hpp>
#include <sake/boost_ext/fusion/algorithm/query/compare.hpp>
#include <sake/boost_ext/fusion/algorithm/query/equal.hpp>
#include <sake/boost_ext/fusion/algorithm/query/not_equal.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/container/tuple_fwd.hpp>
#include <sake/core/functional/operators/less.hpp>
#include <sake/core/math/compare.hpp>
#include <sake/core/math/zero.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>
#include <sake/core/utility/emplacer.hpp>
#include <sake/core/utility/implicitly_defined/default_ctor.hpp>
#include <sake/core/utility/implicitly_defined/mem_fun.hpp>
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
    typedef boost::mpl::vector0<> values_type;
    static std::size_t const static_size = 0;

    SAKE_IMPLICITLY_DEFINED_DEFAULT_CTOR( tuple )

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

    static void
    swap(tuple&)
    { }

    static std::size_t
    hash_value()
    { return static_cast< std::size_t >(0); }
};

} // namespace tuple_adl

/*******************************************************************************
 * make_tuple(T0&& x0, ... ) -> result_of::make_tuple< T0, ... >::type
 ******************************************************************************/

namespace result_of
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_BINARY_PARAMS( SAKE_TUPLE_MAX_SIZE, class T, = void BOOST_PP_INTERCEPT ) >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct make_tuple;

template<>
struct make_tuple<>
{ typedef sake::tuple<> type; };

} // namespace result_of

inline sake::tuple<>
make_tuple()
{ return sake::tuple<>(); }

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... T >
typename result_of::make_tuple< T... >::type
make_tuple(T&&... x)
{ return typename result_of::make_tuple< T... >::type(sake::forward<T>(x)...); }

#else // #if !defined( ... ) && ...

#define SAKE_OVERLOAD_RESULT( n, T_tuple ) \
    result_of::make_tuple< BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_tuple
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    return typename SAKE_OVERLOAD_RESULT( n, T_tuple ) forward_x_tuple ;
#define SAKE_OVERLOAD_FWD_MAX_ARITY SAKE_TUPLE_MAX_SIZE
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && ...

namespace tuple_adl
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
#define class_T0N class... T
#define class_U0N class... U
#define T0N       T...
#define U0N       U...
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
#define class_T0N BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, class T )
#define class_U0N BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, class U )
#define T0N       BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, T )
#define U0N       BOOST_PP_ENUM_PARAMS( SAKE_TUPLE_MAX_SIZE, U )
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

/*******************************************************************************
 * swap(tuple< T0, ... >& x, tuple< U0, ... >& y) -> void
 * hash_value(tuple< T0, ... > const & x) -> std::size_t
 ******************************************************************************/

template< class_T0N >
inline void
swap(sake::tuple< T0N >& x0, sake::tuple< T0N >& x1)
{ x0.swap(x1); }

template< class_T0N >
inline std::size_t
hash_value(sake::tuple< T0N > const & x)
{ return x.hash_value(); }

/*******************************************************************************
 * operator==(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator!=(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * operator<(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 * compare(tuple< T0, ... > const & x, tuple< U0, ... > const & y) -> ...
 ******************************************************************************/

template< class_T0N, class_U0N >
inline typename boost_ext::fusion::result_of::equal<
    sake::tuple< T0N >, sake::tuple< U0N >
>::type
operator==(sake::tuple< T0N > const & x, sake::tuple< U0N > const & y)
{ return boost_ext::fusion::equal(x, y); }

template< class_T0N, class_U0N >
inline typename boost_ext::fusion::result_of::not_equal<
    sake::tuple< T0N >, sake::tuple< U0N >
>::type
operator!=(sake::tuple< T0N > const & x, sake::tuple< U0N > const & y)
{ return boost_ext::fusion::not_equal(x, y); }

template< class_T0N, class_U0N >
inline typename boost_ext::fusion::result_of::compare<
    sake::tuple< T0N >, sake::tuple< U0N >
>::type
compare(sake::tuple< T0N > const & x, sake::tuple< U0N > const & y)
{ return boost_ext::fusion::compare(x, y); }

template< class_T0N, class_U0N >
inline typename operators::result_of::less<
    typename result_of::compare< sake::tuple< T0N > const &, sake::tuple< U0N > const & >::type,
    sake::zero_t
>::type
operator<(sake::tuple< T0N > const & x, sake::tuple< U0N > const & y)
{ return sake::compare(x,y) < sake::zero; }

#undef class_T0N
#undef class_U0N
#undef T0N
#undef U0N

} // namespace tuple_adl

/*******************************************************************************
 * struct tuple< T0, ... >
 * struct result_of::make_tuple< T0, ... >
 ******************************************************************************/

namespace tuple_adl
{

namespace private_
{

template< std::size_t N >
struct at_c_dispatch;

} // namespace private_

} // namespace tuple_adl

#define datan_seq( z, n, data ) ( data ## n )
#define Tn_n( z, n, data ) T ## n _ ## n;
#define comma_i_emplacer_construct_Ti_elem( r, data, i, elem ) \
    BOOST_PP_COMMA_IF( i ) \
    BOOST_PP_CAT( _, i ) (sake::emplacer_construct< BOOST_PP_CAT( T, i ) >(elem))
#define fwd_ref_Un_xn( z, n, data ) SAKE_FWD_REF( U ## n ) x ## n
#define emplacer_assign_n_forward_Un_xn( z, n, data ) \
    sake::emplacer_assign(_ ## n, sake::forward< U ## n >(x ## n));
#define _n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    _ ## n = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));
#define swap_n_other_n( z, n, data ) \
    sake::swap(_ ## n, other._ ## n);
#define hash_combine_x_n( z, n, data ) \
    boost::hash_combine(x, _ ## n);
#define wrapped_parameter_to_reference_remove_qualifiers_Tn( z, n, data ) \
    typename sake::wrapped_parameter_to_reference< \
        typename boost_ext::remove_qualifiers< T ## n >::type >::type

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/container/tuple.hpp>
#include BOOST_PP_ITERATE()

#undef datan_seq
#undef Tn_n
#undef comma_i_emplacer_construct_Ti_elem
#undef fwd_ref_Un_xn
#undef emplacer_assign_n_forward_Un_xn
#undef _n_assign_at_c_n_forward_Sequence_s
#undef swap_n_other_n
#undef hash_combine_x_n
#undef wrapped_parameter_to_reference_remove_qualifiers_Tn

} // namespace sake

#endif // #ifndef SAKE_CORE_CONTAINER_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_FRAME_ITERATION( 1 )

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

#define T0N_seq BOOST_PP_REPEAT( N, datan_seq, T )
#define _0N_seq BOOST_PP_REPEAT( N, datan_seq, _ )
#define T0N_0N  BOOST_PP_REPEAT( N, Tn_n, ~ )

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
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > values_type;
    static std::size_t const static_size = N;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename tuple, T0N_seq )

    SAKE_IMPLICITLY_DEFINED_MEM_FUN(
        tuple,
        ( default_ctor ) ( move_ctor ) ( copy_assign ) ( move_assign ),
        BOOST_PP_SEQ_NIL, _0N_seq
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
        typename boost::disable_if_c< boost::is_base_of<
            tuple, typename boost_ext::remove_qualifiers< U0 >::type
        >::value >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(sake::forward< U0 >(x0)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit tuple(U0& x0,
        typename boost::disable_if_c< boost::is_base_of<
            tuple, typename boost_ext::remove_qualifiers< U0 >::type
        >::value >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(x0))
    { }

    template< class U0 >
    explicit tuple(U0 const & x0,
        typename boost::disable_if_c< boost::is_base_of<
            tuple, typename boost_ext::remove_qualifiers< U0 >::type
        >::value >::type* = 0)
        : _0(sake::emplacer_construct< T0 >(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if N == 1

    // template< class U0, ... >
    // tuple(U0&& x0, ... );
#define SAKE_OVERLOAD_T U
#define SAKE_OVERLOAD_CTOR_NAME \
    tuple
#define SAKE_OVERLOAD_CTOR_INIT_LIST( n, U_tuple, x_tuple, forward_x_tuple ) \
    BOOST_PP_SEQ_FOR_EACH_I( \
        comma_i_emplacer_construct_Ti_elem, \
        ~, \
        BOOST_PP_TUPLE_TO_SEQ( n, forward_x_tuple ) \
    )
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple )
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
        : first (boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
          second(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    tuple(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence& >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

    template< class Sequence >
    tuple(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            tuple, Sequence const & >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class_U0N >
    tuple&
    assign( BOOST_PP_ENUM( N, fwd_ref_Un_xn, ~ ) )
    {
        BOOST_PP_REPEAT( N, emplacer_assign_n_forward_Un_xn, ~ )
        return *this;
    }

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

    void swap(tuple& other)
    { BOOST_PP_REPEAT( N, swap_n_other_n, ~ ) }

    std::size_t
    hash_value() const
    {
        std::size_t x = 0;
        BOOST_PP_REPEAT( N, hash_combine_x_n, ~ )
        return x;
    }
};

} // namespace tuple_adl

namespace result_of
{

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct make_tuple
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct make_tuple< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
{
    typedef sake::tuple< BOOST_PP_ENUM( N,
        wrapped_parameter_to_reference_remove_qualifiers_Tn, ~ ) > type;
};

} // namespace result_of

#undef class_U0N

#undef T0N_seq
#undef _0N_seq
#undef T0N_0N

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING