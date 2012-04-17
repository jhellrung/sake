/*******************************************************************************
 * sake/core/utility/compressed_tuple/compressed_tuple.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct compressed_tuple< T0, ... >
 *
 * swap(compressed_tuple< T0, ... >& x, compressed_tuple< U0, ... >& y) -> void
 * hash_value(compressed_tuple< T0, ... > const & x) -> std::size_t
 *
 * This takes advantage of EBO, similar to compressed_pair.  If at least one
 * type is non-empty, all empty types will contribute nothing to
 * sizeof( compressed_tuple ).
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_COMPRESSED_TUPLE_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_COMPRESSED_TUPLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/compressed_tuple.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/front.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/compressed_tuple/fwd.hpp>
#include <sake/core/utility/compressed_tuple/private/storage.hpp>
#include <sake/core/utility/emplacer/assign.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>

namespace sake
{

/*******************************************************************************
 * struct compressed_tuple<>
 ******************************************************************************/

namespace compressed_tuple_adl
{

template<>
struct compressed_tuple<>
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector<> values_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::vector0<> values_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    static std::size_t const static_size = 0;

    SAKE_MEMBERWISE_MEM_FUN(
        compressed_tuple,
        ( default_ctor ) ( swap ) ( hash_value ),
        BOOST_PP_SEQ_NIL
    )

    template< class Sequence >
    explicit compressed_tuple(Sequence const &,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_tuple, Sequence const & >::value >::type* = 0)
    { }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            compressed_tuple, Sequence const & >::value,
        compressed_tuple&
    >::type
    operator=(Sequence const &)
    { return *this; }
};

/*******************************************************************************
 * struct compressed_tuple< T0, ... >
 ******************************************************************************/

#define at_c_n_data( z, n, data ) \
    boost_ext::fusion::at_c<n>(data)
#define at_c_n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    at_c<n>() = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));
#define fwd_ref_Un_xn( z, n, data ) \
    SAKE_FWD_REF( U ## n ) x ## n
#define emplacer_assign_at_c_n_forward_Un_xn(z, n, data ) \
    emplacer_assign(at_c<n>(), sake::forward< U ## n >(x ## n));

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_COMPRESSED_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/utility/compressed_tuple/compressed_tuple.hpp>
#include BOOST_PP_ITERATE()

#undef at_c_n_data
#undef at_c_n_assign_at_c_n_forward_Sequence_s
#undef fwd_ref_Un_xn
#undef emplacer_assign_at_c_n_forward_Un_xn

} // namespace compressed_tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_COMPRESSED_TUPLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_FRAME_ITERATION( 1 )

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_COMPRESSED_TUPLE_MAX_SIZE
struct compressed_tuple
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_COMPRESSED_TUPLE_MAX_SIZE
struct compressed_tuple< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_COMPRESSED_TUPLE_MAX_SIZE
#if N == 1
    : sake::base_member< T0 >
#endif // #if N == 1
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0N > values_type;
#else // BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > values_type;
#endif // BOOST_NO_VARIADIC_TEMPLATES
    static std::size_t const static_size = N;

#if N == 1

private:
    typedef sake::base_member< T0 > base_member_;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename compressed_tuple,
        (( base_member_ ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename compressed_tuple,
        ( default_ctor ) ( swap ) ( hash_value ),
        (( base_member_ ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit compressed_tuple(U0&& x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< compressed_tuple, U0 >::value
        >::type* = 0)
        : base_member_(sake::forward< U0 >(x0))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    explicit compressed_tuple(U0& x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< compressed_tuple, U0 >::value
        >::type* = 0)
        : base_member_(sake::forward< U0 >(x0))
    { }

    template< class U0 >
    explicit compressed_tuple(U0 const & x0,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< compressed_tuple, U0 >::value
        >::type* = 0)
        : base_member_(sake::forward< U0 >(x0))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#else // #if N == 1

private:
    typedef private_::storage< values_type > m_storage_type;
public:
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename compressed_tuple,
        (( m_storage_type )( m_storage ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename compressed_tuple,
        ( default_ctor ) ( swap ) ( hash_value ),
        (( m_storage_type )( m_storage ))
    )

    // template< class U0, ... >
    // compressed_tuple(U0&& x0, ... );
#define SAKE_OVERLOAD_T U
#define SAKE_OVERLOAD_CTOR_NAME \
    compressed_tuple
#define SAKE_OVERLOAD_CTOR_INIT_LIST( r, n, U_tuple, x_tuple, forward_x_tuple ) \
    m_storage forward_x_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY         N
#if N <= SAKE_COMPRESSED_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY N
#else // #if N <= SAKE_COMPRESSED_TUPLE_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     N
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    N
#endif // #if N <= SAKE_COMPRESSED_TUPLE_PERFECT_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if N == 1

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    compressed_tuple(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_tuple, Sequence >::value >::type* = 0)
#if N == 1
        : base_member_(boost_ext::fusion::front(sake::forward< Sequence >(s)))
#else // #if N == 1
        : m_storage( BOOST_PP_ENUM( N, at_c_n_data, sake::forward< Sequence >(s) ) )
#endif // #if N == 1
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    compressed_tuple(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_tuple, Sequence& >::value >::type* = 0)
#if N == 1
        : base_member_(boost_ext::fusion::front(s))
#else // #if N == 1
        : m_storage( BOOST_PP_ENUM( N, at_c_n_data, s ) )
#endif // #if N == 1
    { }

    template< class Sequence >
    compressed_tuple(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_tuple, Sequence const & >::value >::type* = 0)
#if N == 1
        : base_member_(boost_ext::fusion::front(s))
#else // #if N == 1
        : m_storage( BOOST_PP_ENUM( N, at_c_n_data, s ) )
#endif // #if N == 1
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            compressed_tuple, SAKE_FWD_PARAM( Sequence ) >::value,
        compressed_tuple&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
#if N == 1
        this->member() = boost_ext::fusion::front(sake::forward< Sequence >(s));
#else // #if N == 1
        BOOST_PP_REPEAT( N, at_c_n_assign_at_c_n_forward_Sequence_s, ~ )
#endif // #if N == 1
        return *this;
    }

    template< class_U0N >
    compressed_tuple&
    assign( BOOST_PP_ENUM( N, fwd_ref_Un_xn, ~ ) )
    {
#if N == 1
        sake::emplacer_assign(this->member(), sake::forward< U0 >(x0));
#else // #if N == 1
        BOOST_PP_REPEAT( N, emplacer_assign_at_c_n_forward_Un_xn, ~ )
#endif // #if N == 1
        return *this;
    }

    struct result_of
    {
        template< class This, std::size_t I >
        struct at_c
            : boost_ext::add_reference<
                  typename boost_ext::propagate_const<
                      This, typename boost::mpl::at_c< values_type, I >::type
                  >::type
              >
        { };
        template< class This, class I >
        struct at
            : at_c< This, I::value >
        { };
    };

    template< std::size_t I >
    typename result_of::template at_c< compressed_tuple, I >::type
    at_c()
#if N == 1
    {
        BOOST_STATIC_ASSERT((I == 0));
        return this->member();
    }
#else // #if N == 1
    { return m_storage.template at_c<I>(); }
#endif // #if N == 1
    template< std::size_t I >
    typename result_of::template at_c< compressed_tuple const, I >::type
    at_c() const
#if N == 1
    {
        BOOST_STATIC_ASSERT((I == 0));
        return this->member();
    }
#else // #if N == 1
    { return m_storage.template at_c<I>(); }
#endif // #if N == 1

    template< class I >
    typename result_of::template at< compressed_tuple, I >::type
    at()
    { return at_c< I::value >(); }
    template< class I >
    typename result_of::template at< compressed_tuple const, I >::type
    at() const
    { return at_c< I::value >(); }

#if N != 1
private:
    m_storage_type m_storage;
#endif // #if N != 1
};

#undef class_T0N
#undef class_U0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING