/*******************************************************************************
 * sake/core/data_structures/pair/pair.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct pair< T0, T1 >
 *
 * pair_adl::swap(pair< T0, T1 >& x, pair< T0, T1 >& y) -> void
 * pair_adl::hash_value(pair< T0, T1 > const & x) -> std::size_t
 *
 * A sake::pair is nearly functionally identical to a std::pair, except that it
 * handles reference types and is emplacer- and move-aware.
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP

#include <cstddef>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <sake/boost_ext/fusion/adapted/pair.hpp>

#include <sake/core/data_structures/pair/fwd.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>

#define SAKE_PAIR_INCLUDE_HEADERS
#include <sake/core/data_structures/pair/private/value_constructor.ipp>
#include <sake/core/data_structures/pair/private/sequence_constructor.ipp>
#include <sake/core/data_structures/pair/private/operator_assign.ipp>
#include <sake/core/data_structures/pair/private/assign.ipp>
#undef SAKE_PAIR_INCLUDE_HEADERS

namespace sake
{

/*******************************************************************************
 * struct pair< T0, T1 >
 ******************************************************************************/

namespace pair_adl
{

template< class T0, class T1 >
struct pair
{
    typedef T0 first_type;
    typedef T1 second_type;
    typedef boost::mpl::vector2< T0, T1 > values_type;

    T0 first;
    T1 second;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename pair,
        (( T0 )( first )) (( T1 )( second ))
    )

    SAKE_MEMBERWISE_MEM_FUN(
        typename pair,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( T0 )( first )) (( T1 )( second ))
    )

private:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;
public:

#if 0 // for exposition purposes only
    template< class U0, class U1 >
    pair(U0&& x0, U1&& x1,
        typename value_constructor_enabler< U0, U1 >::type* = 0);

    template< class Sequence >
    pair(Sequence&& s,
        typename sequence_constructor_enabler< Sequence >::type* = 0);

    template< class Sequence >
    typename operator_assign_enabler< Sequence >::type // -> pair&
    operator=(Sequence&& s);

    template< class U0, class U1 >
    typename assign_enabler< U0, U1 >::type // -> void
    assign(U0&& x0, U1&& x1);
#endif
#define SAKE_PAIR_DEFINE_MEMBERS
#include <sake/core/data_structures/pair/private/value_constructor.ipp>
#include <sake/core/data_structures/pair/private/sequence_constructor.ipp>
#include <sake/core/data_structures/pair/private/operator_assign.ipp>
#include <sake/core/data_structures/pair/private/assign.ipp>
#undef SAKE_PAIR_DEFINE_MEMBERS

};

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP
