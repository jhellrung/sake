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

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_empty.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/compressed_tuple.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/front.hpp>
#include <sake/boost_ext/mpl/advance_c.hpp>
#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/mpl/as_vector.hpp>
#include <sake/boost_ext/mpl/reverse_adjacent_find_index.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/propagate_const.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/static_clamp.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/compressed_tuple/fwd.hpp>
#include <sake/core/utility/emplacer/assign.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>
#include <sake/core/utility/swap.hpp>

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
        ( default_ctor )
        ( swap )
        ( hash_value ),
        BOOST_PP_SEQ_NIL, BOOST_PP_SEQ_NIL
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

namespace private_
{

template<
    class Sequence,
    bool = boost_ext::mpl::any< Sequence, boost::mpl::quote1< boost::is_empty > >::value
>
struct storage;

template<
    std::size_t I, std::size_t J,
    bool = (I < J)
>
struct package;

template<
    std::size_t I, std::size_t J,
    int = (J+0 <= I) + (J+1 <= I) + (J+2 <= I)
>
struct at_c_dispatch;

} // namespace private_

#define at_c_n_data( z, n, data ) \
    boost_ext::fusion::at_c<n>(data)
#define at_c_n_assign_at_c_n_forward_Sequence_s( z, n, data ) \
    at_c<n>() = boost_ext::fusion::at_c<n>(sake::forward< Sequence >(s));
#define fwd_ref_Un_xn( z, n, data ) \
    SAKE_FWD_REF( U ## n ) x ## n
#define emplacer_assign_at_c_n_forward_Un_xn(z, n, data ) \
    emplacer_assign(at_c<n>(), sake::forward< U ## n >(x ## n));
#define swap_at_c_n_other_at_c_n( z, n, data ) \
    sake::swap(at_c<n>(), other.template at_c<n>());
#define hash_combine_x_at_c_n( z, n, data ) \
    boost::hash_combine(x, at_c<n>());

#define fwd2_ref_Un_xn( z, n, data ) \
    SAKE_FWD2_REF( U ## n ) x ## n
#define forward_Un_xn( z, n, data ) \
    sake::forward< U ## n >(x ## n)
#define package_n_ci_apply_forward_Un_xn_forward_Unp1_xnp1( z, n, data ) \
    private_::package<n,j>::apply( \
        sake::forward< U ## n >(x ## n), \
        sake::forward< BOOST_PP_CAT( U, BOOST_PP_INC( n ) ) >( \
            BOOST_PP_CAT( x, BOOST_PP_INC( n ) ) ) \
    )

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_COMPRESSED_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/utility/compressed_tuple/compressed_tuple.hpp>
#include BOOST_PP_ITERATE()

#undef fwd2_ref_Un_xn
#undef forward_Un_xn
#undef package_n_ci_apply_forward_Un_xn_forward_Unp1_xnp1

#undef at_c_n_data
#undef at_c_n_assign_at_c_n_forward_Sequence_s
#undef fwd_ref_Un_xn
#undef emplacer_assign_at_c_n_forward_Un_xn
#undef swap_at_c_n_other_at_c_n
#undef hash_combine_x_at_c_n

namespace private_
{

template< class T0, class T1, bool _ >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost_ext::mpl::vector< T0, T1 >, _ >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost::mpl::vector2< T0, T1 >, _ >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0, T1 > values_type;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::vector2< T0, T1 > values_type;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    SAKE_BASIC_MOVABLE_COPYABLE( storage )

private:
    typedef sake::compressed_pair< T0, T1 > m_pair_type;
public:
    SAKE_MEMBERWISE_MEM_FUN(
        storage,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        BOOST_PP_SEQ_NIL, (( m_pair_type, m_pair ))
    )

    template< class U0, class U1 >
    storage(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : m_pair(sake::forward< U0 >(x0), sake::forward< U1 >(x1))
    { }

    template< std::size_t I >
    typename boost_ext::add_reference<
        typename boost::mpl::at_c< values_type, I >::type
    >::type
    at_c()
    { return boost_ext::fusion::at_c<I>(m_pair); }

    template< std::size_t I >
    typename boost_ext::add_reference_add_const<
        typename boost::mpl::at_c< values_type, I >::type
    >::type
    at_c() const
    { return boost_ext::fusion::at_c<I>(m_pair); }

private:
    m_pair_type m_pair;
};

/*******************************************************************************
 * struct package< std::size_t I, std::size_t J >
 *
 * The apply static member function returns x0 if I < J, packages x0 and
 * x1 together if I == J, and returns x1 if I > J.
 ******************************************************************************/

template< std::size_t I, std::size_t J >
struct package< I, J, true >
{
    template< class T0, class T1 >
    static SAKE_FWD2_REF( T0 )
    apply(SAKE_FWD2_REF( T0 ) x0, T1 const &)
    { return sake::forward< T0 >(x0); }
};

template< std::size_t J >
struct package< J, J, false >
{
    template< class T0, class T1 >
    // Hack: const-qualified so it can bind to SAKE_FWD2_REF in C++03.
    static sake::emplacer< void ( SAKE_FWD2_PARAM( T0 ), SAKE_FWD2_PARAM( T1 ) ) > const
    apply(SAKE_FWD2_REF( T0 ) x0, SAKE_FWD2_REF( T1 ) x1)
    {
        typedef sake::emplacer< void ( SAKE_FWD2_PARAM( T0 ), SAKE_FWD2_PARAM( T1 ) ) > result_type;
        return result_type(sake::forward< T0 >(x0), sake::forward< T1 >(x1));
    }
};

template< std::size_t I, std::size_t J >
struct package< I, J, false >
{
    template< class T0, class T1 >
    static SAKE_FWD2_REF( T1 )
    apply(T0 const &, SAKE_FWD2_REF( T1 ) x1)
    { return sake::forward< T1 >(x1); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,0>
{
    BOOST_STATIC_ASSERT((I < J+0));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::values_type, I
            >::type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<I>(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,1>
{
    BOOST_STATIC_ASSERT((I == J+0));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::values_type, J
            >::type::first_type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<J>().first(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,2>
{
    BOOST_STATIC_ASSERT((I == J+1));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::values_type, J
            >::type::second_type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<J>().second(); }
};

template< std::size_t I, std::size_t J >
struct at_c_dispatch<I,J,3>
{
    BOOST_STATIC_ASSERT((I > J+1));
    template< class Storage >
    static typename boost_ext::add_reference<
        typename boost_ext::propagate_const<
            Storage,
            typename boost::mpl::at_c<
                typename Storage::values_type, I-1
            >::type
        >::type
    >::type
    apply(Storage& storage)
    { return storage.template at_c<I-1>(); }
};

} // namespace private_

} // namespace compressed_tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_HPP

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

    SAKE_BASIC_MOVABLE_COPYABLE( compressed_tuple )

#if N == 1

private:
    typedef sake::base_member< T0 > base_member_;
public:
    SAKE_MEMBERWISE_MEM_FUN(
        compressed_tuple,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        ( base_member_ ), BOOST_PP_SEQ_NIL
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
    SAKE_MEMBERWISE_MEM_FUN(
        compressed_tuple,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        BOOST_PP_SEQ_NIL, (( m_storage_type, m_storage ))
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

    void swap(compressed_tuple& other)
    { BOOST_PP_REPEAT( N, swap_at_c_n_other_at_c_n, ~ ); }
    inline friend
    void swap(compressed_tuple& x, compressed_tuple& y)
    { x.swap(y); }

    std::size_t hash_value() const
    {
        std::size_t x = 0;
        BOOST_PP_REPEAT( N, hash_combine_x_at_c_n, ~ )
        return x;
    }
    inline friend
    std::size_t hash_value(compressed_tuple const & x)
    { return x.hash_value(); }

private:
#if N != 1
    m_storage_type m_storage;
#endif // #if N != 1
};

#if N > 2

namespace private_
{

template< class_T0N >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost_ext::mpl::vector< T0N >, false >
#else // BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost::mpl::BOOST_PP_CAT( vector, N )< T0N >, false >
#endif // BOOST_NO_VARIADIC_TEMPLATES
    : sake::tuple< T0N >
{
    SAKE_BASIC_MOVABLE_COPYABLE( storage )

private:
    typedef sake::tuple< T0N > tuple_;
public:
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_ctor ) ( move_ctor ) ( copy_assign_if_any_umc ) ( move_assign ),
        ( tuple_ ), BOOST_PP_SEQ_NIL
    )

    template< class_U0N >
    storage( BOOST_PP_ENUM( N, fwd2_ref_Un_xn, ~ ) )
        : tuple_( BOOST_PP_ENUM( N, forward_Un_xn, ~ ) )
    { }
};

template< class_T0N >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost_ext::mpl::vector< T0N >, true >
#else // BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost::mpl::BOOST_PP_CAT( vector, N )< T0N >, true >
#endif // BOOST_NO_VARIADIC_TEMPLATES
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0N > values_type;
#else // BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > values_type;
#endif // BOOST_NO_VARIADIC_TEMPLATES

    SAKE_BASIC_MOVABLE_COPYABLE( storage )

private:
    static std::size_t const j = sake::static_clamp_c<
        0,
        boost_ext::mpl::reverse_adjacent_find_index<
            values_type,
            boost::mpl::not_equal_to<
                boost::is_empty< boost::mpl::_1 >,
                boost::is_empty< boost::mpl::_2 >
            >
        >::value,
        N - 2
    >::value;
    // Assert that at least one of T[j] and T[j+1] are empty.
    BOOST_STATIC_ASSERT((
        boost::is_empty< typename boost::mpl::at_c< values_type, j+0 >::type >::value
     || boost::is_empty< typename boost::mpl::at_c< values_type, j+1 >::type >::value
    ));
    // Assert that if *both* T[j] and T[j+1] are empty, then *all* T[i] must be
    // empty.
    BOOST_STATIC_ASSERT((
       !boost::is_empty< typename boost::mpl::at_c< values_type, j+0 >::type >::value
    || !boost::is_empty< typename boost::mpl::at_c< values_type, j+1 >::type >::value
    ||  boost_ext::mpl::all< values_type, boost::mpl::quote1< boost::is_empty > >::value
    ));
    typedef typename boost::mpl::begin< values_type >::type begin_values_type;
    typedef typename boost::mpl::end< values_type >::type end_values_type;
    typedef private_::storage<
        // candidate for abstracting into a mpl::flatten algorithm?
        typename boost_ext::mpl::as_vector<
            // This might *look* complicated, but it's really not too bad.
            // We just want to form a Boost.MPL sequence consisting of
            //     T[0], ..., T[j-1],
            //     compressed_pair< T[j], T[j+1] >,
            //     T[j+2], ..., T[N-1]
            typename boost::mpl::copy<
                // T[j+2], ..., T[N-1]
                boost::mpl::iterator_range<
                    typename boost_ext::mpl::advance_c< begin_values_type, j+2 >::type,
                    end_values_type
                >,
                boost::mpl::back_inserter<
                    typename boost::mpl::push_back<
                        typename boost::mpl::copy<
                            // T[0], ..., T[j-1]
                            boost::mpl::iterator_range<
                                begin_values_type,
                                typename boost_ext::mpl::advance_c< begin_values_type, j+0 >::type
                            >,
                            boost::mpl::back_inserter<
#ifndef BOOST_NO_VARIADIC_TEMPLATES
                                boost_ext::mpl::vector<>
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
                                boost::mpl::vector0<>
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
                            >
                        >::type,
                        // compressed_pair< T[j], T[j+1] >
                        sake::compressed_pair<
                            typename boost::mpl::at_c< values_type, j+0 >::type,
                            typename boost::mpl::at_c< values_type, j+1 >::type
                        >
                    >::type
                >
            >::type
        >::type
    > m_storage_type;
public:

    SAKE_MEMBERWISE_MEM_FUN(
        storage,
        ( default_ctor ) ( move_ctor ) ( move_assign ),
        BOOST_PP_SEQ_NIL, (( m_storage_type, m_storage ))
    )

    template< class_U0N >
    storage( BOOST_PP_ENUM( N, fwd2_ref_Un_xn, ~ ) )
        : m_storage( BOOST_PP_ENUM( BOOST_PP_DEC( N ),
              package_n_ci_apply_forward_Un_xn_forward_Unp1_xnp1, ~ ) )
    { }

    template< std::size_t I >
    typename boost_ext::add_reference<
        typename boost::mpl::at_c< values_type, I >::type
    >::type
    at_c()
    { return private_::at_c_dispatch<I,j>::apply(m_storage); }

    template< std::size_t I >
    typename boost_ext::add_reference_add_const<
        typename boost::mpl::at_c< values_type, I >::type
    >::type
    at_c() const
    { return private_::at_c_dispatch<I,j>::apply(m_storage); }

private:
    m_storage_type m_storage;
};

} // namespace private_

#endif // #if N > 2

#undef class_T0N
#undef class_U0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
