/*******************************************************************************
 * sake/core/data_structures/pair/pair.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct pair< T0, T1 >
 *
 * swap(pair< T0, T1 >& x, pair< T0, T1 >& y) -> void
 * hash_value(pair< T0, T1 > const & x) -> std::size_t
 *
 * A sake::pair is nearly functionally identical to a std::pair, except that it
 * handles reference types and is emplacer- and move-aware.
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP

#include <cstddef>

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/pair.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>

#include <sake/core/data_structures/pair/fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/define_natural/mem_fun.hpp>
#include <sake/core/utility/emplacer.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>

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

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename pair, ( T0 ) ( T1 ) )

    SAKE_DEFINE_NATURAL_MEM_FUN(
        pair,
        ( default_ctor ) ( move_ctor )
        ( copy_assign ) ( move_assign )
        ( swap )
        ( hash_value ),
        BOOST_PP_SEQ_NIL, (( T0, first )) (( T1, second ))
    )

    pair(sake::emplacer< void ( ) >, sake::emplacer< void ( ) >)
    { }

private:
    typedef typename boost::remove_cv< T0 >::type nocv0_type;
    typedef typename boost::remove_cv< T1 >::type nocv1_type;
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    pair(U0&& x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< nocv0_type >(sake::forward< U0 >(x0)))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1&& x1)
        : second(sake::emplacer_construct< nocv1_type >(sake::forward< U1 >(x1)))
    { }

    template< class U0, class U1 >
    pair(U0&& x0, U1&& x1)
        : first (sake::emplacer_construct< nocv0_type >(sake::forward< U0 >(x0))),
          second(sake::emplacer_construct< nocv1_type >(sake::forward< U1 >(x1)))
    { }

    template< class Sequence >
    pair(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
          second(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0 >
    pair(U0& x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< nocv0_type >(x0))
    { }

    template< class U0 >
    pair(U0 const & x0, sake::emplacer< void ( ) >)
        : first (sake::emplacer_construct< nocv0_type >(x0))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1& x1)
        : second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class U1 >
    pair(sake::emplacer< void ( ) >, U1 const & x1)
        : second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    pair(U0& x0, U1& x1)
        : first (sake::emplacer_construct< nocv0_type >(x0)),
          second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    pair(U0& x0, U1 const & x1)
        : first (sake::emplacer_construct< nocv0_type >(x0)),
          second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    pair(U0 const & x0, U1& x1)
        : first (sake::emplacer_construct< nocv0_type >(x0)),
          second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class U0, class U1 >
    pair(U0 const & x0, U1 const & x1)
        : first (sake::emplacer_construct< nocv0_type >(x0)),
          second(sake::emplacer_construct< nocv1_type >(x1))
    { }

    template< class Sequence >
    pair(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence& >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

    template< class Sequence >
    pair(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            pair, Sequence const & >::value >::type* = 0)
        : first (boost_ext::fusion::at_c<0>(s)),
          second(boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    pair&
    assign(SAKE_FWD_REF( U0 ) x0, SAKE_FWD_REF( U1 ) x1)
    {
        sake::emplacer_assign(first,  sake::forward< U0 >(x0));
        sake::emplacer_assign(second, sake::forward< U1 >(x1));
        return *this;
    }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            pair, SAKE_FWD_PARAM( Sequence ) >::value,
        pair&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        first  = boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s));
        second = boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s));
        return *this;
    }
};

/*******************************************************************************
 * swap(pair< T0, T1 >& x, pair< T0, T1 >& y) -> void
 * hash_value(pair< T0, T1 > const & x) -> std::size_t
 ******************************************************************************/

template< class T0, class T1 >
inline void
swap(sake::pair< T0, T1 >& x, sake::pair< T0, T1 >& y)
{ x.swap(y); }

template< class T0, class T1 >
inline std::size_t
hash_value(sake::pair< T0, T1 > const & x)
{ return x.hash_value(); }

} // namespace pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_PAIR_HPP
