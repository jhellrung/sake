/*******************************************************************************
 * sake/core/data_structures/pair/private/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PRIVATE_BASE_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_PRIVATE_BASE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>

namespace sake
{

namespace pair_adl
{

namespace private_
{

struct sequence_rv_sink_default_tag { };

template< class T0, class T1 >
struct base
{
    T0 first;
    T1 second;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename base,
        (( T0 )( first ))
        (( T1 )( second ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename base,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T0 )( first ))
        (( T1 )( second ))
    )

protected:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;

    template< class V0, class V1 >
    base(sake::emplacer< V0 ( ) >, sake::emplacer< V1 ( ) >)
    { }

    template< class U0, class V1 >
    base(SAKE_FWD2_REF( U0 ) x0, sake::emplacer< V1 ( ) >)
        : first(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0)))
    { }

    template< class V0, class U1 >
    base(sake::emplacer< V0 ( ) >, SAKE_FWD2_REF( U1 ) x1)
        : second(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

    template< class U0, class U1 >
    base(SAKE_FWD2_REF( U0 ) x0, SAKE_FWD2_REF( U1 ) x1)
        : first(sake::emplacer_constructible< nocv0_type >(sake::forward< U0 >(x0))),
          second(sake::emplacer_constructible< nocv1_type >(sake::forward< U1 >(x1)))
    { }

#ifdef BOOST_NO_RVALUE_REFERENCES
    template< class SequenceRVSinkDefault >
    base(private_::sequence_rv_sink_default_tag, SequenceRVSinkDefault & s)
        : first (s[boost::integral_constant< std::size_t, 0 >()]),
          second(s[boost::integral_constant< std::size_t, 1 >()])
    { }
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES
};

} // namespace private_

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PRIVATE_BASE_HPP
