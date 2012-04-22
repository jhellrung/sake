/*******************************************************************************
 * sake/core/utility/compressed_pair/compressed_pair.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct compressed_pair< T0, T1 >
 *
 * compressed_pair_adl::swap(
 *     compressed_pair< T0, T1 >& x, compressed_pair< T0, T1 >& y) -> void
 * compressed_pair_adl::hash_value(
 *     compressed_pair< T0, T1 > const & x) -> std::size_t
 *
 * A sake::compressed_pair is nearly functionally identical to a
 * boost::compressed_pair, except that it is emplace- and move-aware.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_COMPRESSED_PAIR_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_PAIR_COMPRESSED_PAIR_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/fusion/adapted/compressed_pair.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/compressed_pair/fwd.hpp>
#include <sake/core/utility/compressed_pair/private/storage.hpp>
#include <sake/core/utility/emplacer/assign.hpp>
#include <sake/core/utility/emplacer/make.hpp>
#include <sake/core/utility/emplacer/wrap.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>
#include <sake/core/utility/private/is_compatible_sequence.hpp>
#include <sake/core/utility/swap.hpp>

namespace sake
{

/*******************************************************************************
 * struct compressed_pair< T0, T1 >
 ******************************************************************************/

namespace compressed_pair_adl
{

template< class T0, class T1 >
struct compressed_pair
{
    typedef T0 first_type;
    typedef T1 second_type;
    typedef boost::mpl::vector2< T0, T1 > values_type;

private:
    typedef private_::storage< T0, T1 > m_storage_type;
public:

    SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
        typename compressed_pair,
        (( m_storage_type )( m_storage ))
    )
    SAKE_MEMBERWISE_MEM_FUN(
        typename compressed_pair,
        ( default_constructor ) ( swap ) ( hash_value ),
        (( m_storage_type )( m_storage ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0&& x0, U1&& x1)
        : m_storage(
              sake::emplacer_wrap(sake::forward< U0 >(x0)),
              sake::emplacer_wrap(sake::forward< U1 >(x1))
          )
    { }

    template< class Sequence >
    compressed_pair(Sequence&& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s))),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
          )
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0& x0, U1& x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0& x0, U1 const & x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1& x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1 const & x1)
        : m_storage(sake::emplacer_wrap(x0), sake::emplacer_wrap(x1))
    { }

    template< class Sequence >
    compressed_pair(Sequence& s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence& >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(s)),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(s))
          )
    { }

    template< class Sequence >
    compressed_pair(Sequence const & s,
        typename boost::enable_if_c< utility_private::is_compatible_sequence<
            compressed_pair, Sequence const & >::value >::type* = 0)
        : m_storage(
              sake::make_emplacer(boost_ext::fusion::at_c<0>(s)),
              sake::make_emplacer(boost_ext::fusion::at_c<1>(s))
          )
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    void assign(SAKE_FWD_REF( U0 ) x0, SAKE_FWD_REF( U1 ) x1)
    {
        sake::emplacer_assign(first(),  sake::forward< U0 >(x0));
        sake::emplacer_assign(second(), sake::forward< U1 >(x1));
    }

    template< class Sequence >
    typename boost::enable_if_c<
        utility_private::is_compatible_sequence<
            compressed_pair, SAKE_FWD_PARAM( Sequence ) >::value,
        compressed_pair&
    >::type
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        first()  = boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s));
        second() = boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s));
        return *this;
    }

    typename boost_ext::add_reference< T0 >::type
    first()
    { return m_storage.first(); }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const
    { return m_storage.first(); }

    typename boost_ext::add_reference< T1 >::type
    second()
    { return m_storage.second(); }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const
    { return m_storage.second(); }

private:
    m_storage_type m_storage;
};

} // namespace compressed_pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_COMPRESSED_PAIR_HPP
