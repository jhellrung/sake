/*******************************************************************************
 * sake/core/utility/compressed_pair/private/storage.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_PRIVATE_STORAGE_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_PAIR_PRIVATE_STORAGE_HPP

#include <boost/type_traits/is_empty.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>

namespace sake
{

namespace compressed_pair_adl
{

namespace private_
{

template<
    class T0, class T1,
    bool = boost::is_empty< T0 >::value,
    bool = boost::is_empty< T1 >::value
>
struct storage;

template< class T0, class T1 >
struct storage< T0, T1, false, false >
{
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( T0 )( m_x0 )) (( T1 )( m_x1 ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T0 )( m_x0 )) (( T1 )( m_x1 ))
    )

private:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;
public:

    template< class V0, class V1 >
    storage(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >)
    { }
    template< class U0, class V1 >
    storage(SAKE_FWD2_REF( U0 ) x0, sake::emplacer< V1 ( ) >)
        : m_x0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    storage(sake::emplacer< V0 ( ) >, SAKE_FWD2_REF( U1 ) x1)
        : m_x1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0, class U1 >
    storage(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : m_x0(sake::emplacer_constructible< nocv0_type >(x0)),
          m_x1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    typename boost_ext::add_reference< T0 >::type
    first()       { return m_x0; }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const { return m_x0; }

    typename boost_ext::add_reference< T1 >::type
    second()       { return m_x1; }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const { return m_x1; }

private:
    T0 m_x0;
    T1 m_x1;
};

template< class T0, class T1, bool _ >
struct storage< T0, T1, true, _ >
    : private boost::remove_cv< T0 >::type
{
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( T0 )) (( T1 )( m_x1 ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T0 )) (( T1 )( m_x1 ))
    )

private:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;
public:

    template< class V0, class V1 >
    storage(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >)
    { }
    template< class U0, class V1 >
    storage(SAKE_FWD2_REF( U0 ) x0, sake::emplacer< V1 ( ) >)
        : T0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    storage(sake::emplacer< V0 ( ) >, SAKE_FWD2_REF( U1 ) x1)
        : m_x1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0, class U1 >
    storage(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : T0(sake::emplacer_constructible< nocv0_type >(x0)),
          m_x1(sake::emplacer_constructible< nocv1_type >(x1))
    { }

    T0       & first()       { return *this; }
    T0 const & first() const { return *this; }

    typename boost_ext::add_reference< T1 >::type
    second()       { return m_x1; }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const { return m_x1; }

private:
    T1 m_x1;
};

template< class T0, class T1 >
struct storage< T0, T1, false, true >
    : private boost::remove_cv< T1 >::type
{
    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename storage,
        (( T1 )) (( T0 )( m_x0 ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename storage,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T1 )) (( T0 )( m_x0 ))
    )

private:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;
public:

    template< class V0, class V1 >
    storage(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >)
    { }
    template< class U0, class V1 >
    storage(SAKE_FWD2_REF( U0 ) x0, sake::emplacer< V1 ( ) >)
        : m_x0(sake::emplacer_constructible< nocv0_type >(x0))
    { }
    template< class V0, class U1 >
    storage(sake::emplacer< V0 ( ) >, SAKE_FWD2_REF( U1 ) x1)
        : T1(sake::emplacer_constructible< nocv1_type >(x1))
    { }
    template< class U0, class U1 >
    storage(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : T1(sake::emplacer_constructible< nocv1_type >(x1)),
          m_x0(sake::emplacer_constructible< nocv0_type >(x0))
    { }

    typename boost_ext::add_reference< T0 >::type
    first()       { return m_x0; }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const { return m_x0; }

    T1       & second()       { return *this; }
    T1 const & second() const { return *this; }

private:
    T0 m_x0;
};

} // namespace private_

} // namespace compressed_pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_PRIVATE_STORAGE_HPP
