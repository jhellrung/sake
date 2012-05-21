/*******************************************************************************
 * sake/core/utility/compressed_tuple/private/storage.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_STORAGE_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_STORAGE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/not_equal_to.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_empty.hpp>

#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/mpl/advance_c.hpp>
#include <sake/boost_ext/mpl/all.hpp>
#include <sake/boost_ext/mpl/any.hpp>
#include <sake/boost_ext/mpl/as_vector.hpp>
#include <sake/boost_ext/mpl/reverse_adjacent_find_index.hpp>
#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/math/static_clamp.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/compressed_pair/compressed_pair.hpp>
#include <sake/core/utility/compressed_tuple/fwd.hpp>
#include <sake/core/utility/compressed_tuple/private/at_c_dispatch.hpp>
#include <sake/core/utility/compressed_tuple/private/package.hpp>

namespace sake
{

namespace compressed_tuple_adl
{

namespace private_
{

template<
    class Sequence,
    bool = boost_ext::mpl::any<
               Sequence,
               boost::mpl::quote1< boost::is_empty >
           >::value
>
struct storage;

template< class T0, class T1, bool _ >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost_ext::mpl::vector< T0, T1 >, _ >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost::mpl::vector2< T0, T1 >, _ >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
{
#ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost_ext::mpl::vector< T0, T1 > value_types;
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::vector2< T0, T1 > value_types;
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

private:
    typedef sake::compressed_pair< T0, T1 > m_pair_type;
public:

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( m_pair_type )( m_pair ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( m_pair_type )( m_pair ))
    )

    template< class U0, class U1 >
    storage(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : m_pair(sake::forward< U0 >(x0), sake::forward< U1 >(x1))
    { }

    template< std::size_t I >
    typename boost_ext::add_reference<
        typename boost::mpl::at_c< value_types, I >::type
    >::type
    at_c()
    { return boost_ext::fusion::at_c<I>(m_pair); }

    template< std::size_t I >
    typename boost_ext::add_reference_add_const<
        typename boost::mpl::at_c< value_types, I >::type
    >::type
    at_c() const
    { return boost_ext::fusion::at_c<I>(m_pair); }

private:
    m_pair_type m_pair;
};


#if SAKE_COMPRESSED_TUPLE_MAX_SIZE > 2

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

#define BOOST_PP_ITERATION_LIMITS ( 3, SAKE_COMPRESSED_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/utility/compressed_tuple/private/storage.hpp>
#include BOOST_PP_ITERATE()

#undef fwd2_ref_Un_xn
#undef forward_Un_xn
#undef package_n_ci_apply_forward_Un_xn_forward_Unp1_xnp1

#endif // #if SAKE_COMPRESSED_TUPLE_MAX_SIZE > 2

} // namespace private_

} // namespace compressed_tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_STORAGE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define class_U0N BOOST_PP_ENUM_PARAMS( N, class U )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

template< class_T0N >
#ifndef BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost_ext::mpl::vector< T0N >, false >
#else // BOOST_NO_VARIADIC_TEMPLATES
struct storage< boost::mpl::BOOST_PP_CAT( vector, N )< T0N >, false >
#endif // BOOST_NO_VARIADIC_TEMPLATES
    : sake::tuple< T0N >
{
private:
    typedef sake::tuple< T0N > tuple_;
public:

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( tuple_ ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( tuple_ ))
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
    typedef boost_ext::mpl::vector< T0N > value_types;
#else // BOOST_NO_VARIADIC_TEMPLATES
    typedef boost::mpl::BOOST_PP_CAT( vector, N )< T0N > value_types;
#endif // BOOST_NO_VARIADIC_TEMPLATES

private:
    static std::size_t const j = sake::static_clamp_c<
        0,
        boost_ext::mpl::reverse_adjacent_find_index<
            value_types,
            boost::mpl::not_equal_to<
                boost::is_empty< boost::mpl::_1 >,
                boost::is_empty< boost::mpl::_2 >
            >
        >::value,
        N - 2
    >::value;
    // Assert that at least one of T[j] and T[j+1] are empty.
    BOOST_STATIC_ASSERT((
        boost::is_empty< typename boost::mpl::at_c< value_types, j+0 >::type >::value
     || boost::is_empty< typename boost::mpl::at_c< value_types, j+1 >::type >::value
    ));
    // Assert that if *both* T[j] and T[j+1] are empty, then *all* T[i] must be
    // empty.
    BOOST_STATIC_ASSERT((
       !boost::is_empty< typename boost::mpl::at_c< value_types, j+0 >::type >::value
    || !boost::is_empty< typename boost::mpl::at_c< value_types, j+1 >::type >::value
    ||  boost_ext::mpl::all< value_types, boost::mpl::quote1< boost::is_empty > >::value
    ));
    typedef typename boost::mpl::begin< value_types >::type begin_values;
    typedef typename boost::mpl::end< value_types >::type end_values;
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
                    typename boost_ext::mpl::advance_c< begin_values, j+2 >::type,
                    end_values
                >,
                boost::mpl::back_inserter<
                    typename boost::mpl::push_back<
                        typename boost::mpl::copy<
                            // T[0], ..., T[j-1]
                            boost::mpl::iterator_range<
                                begin_values,
                                typename boost_ext::mpl::advance_c< begin_values, j+0 >::type
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
                            typename boost::mpl::at_c< value_types, j+0 >::type,
                            typename boost::mpl::at_c< value_types, j+1 >::type
                        >
                    >::type
                >
            >::type
        >::type
    > m_storage_type;
public:

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( m_storage_type )( m_storage ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( m_storage_type )( m_storage ))
    )

    template< class_U0N >
    storage( BOOST_PP_ENUM( N, fwd2_ref_Un_xn, ~ ) )
        : m_storage( BOOST_PP_ENUM( BOOST_PP_DEC( N ),
              package_n_ci_apply_forward_Un_xn_forward_Unp1_xnp1, ~ ) )
    { }

    template< std::size_t I >
    typename boost_ext::add_reference<
        typename boost::mpl::at_c< value_types, I >::type
    >::type
    at_c()
    { return private_::at_c_dispatch<I,j>::apply(m_storage); }

    template< std::size_t I >
    typename boost_ext::add_reference_add_const<
        typename boost::mpl::at_c< value_types, I >::type
    >::type
    at_c() const
    { return private_::at_c_dispatch<I,j>::apply(m_storage); }

private:
    m_storage_type m_storage;
};

#undef class_T0N
#undef class_U0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
