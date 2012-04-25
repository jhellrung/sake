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
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <sake/boost_ext/fusion/adapted/compressed_pair.hpp>
#include <sake/boost_ext/fusion/sequence/intrinsic/at.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/data_structures/tuple/private/operator_assign_enable.hpp>
#include <sake/core/data_structures/tuple/private/sequence_constructor_enable.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/compressed_pair/fwd.hpp>
#include <sake/core/utility/compressed_pair/private/storage.hpp>
#include <sake/core/utility/memberwise/mem_fun.hpp>

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

private:
    template< class Sequence >
    struct sequence_constructor_enabler
        : sake::tuple_private::sequence_constructor_enabler<
              compressed_pair, Sequence >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0&& x0, U1&& x1)
        : m_storage(sake::forward< U0 >(x0), sake::forward< U1 >(x1))
    { }

    template< class Sequence >
    compressed_pair(Sequence&& s,
        typename sequence_constructor_enabler< Sequence >::type* = 0)
        : m_storage(boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s)),
                    boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s)))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U0, class U1 >
    compressed_pair(U0& x0, U1& x1)
        : m_storage(x0, x1)
    { }
    template< class U0, class U1 >
    compressed_pair(U0& x0, U1 const & x1)
        : m_storage(x0, x1)
    { }
    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1& x1)
        : m_storage(x0, x1)
    { }
    template< class U0, class U1 >
    compressed_pair(U0 const & x0, U1 const & x1)
        : m_storage(x0, x1)
    { }

    template< class Sequence >
    compressed_pair(Sequence& s,
        typename sequence_constructor_enabler<
            typename boost_ext::remove_rvalue_reference< Sequence& >::type
        >::type* = 0)
        : m_storage(boost_ext::fusion::at_c<0>(s),
                    boost_ext::fusion::at_c<1>(s))
    { }

    template< class Sequence >
    compressed_pair(Sequence const & s,
        typename sequence_constructor_enabler< Sequence const & >::type* = 0)
        : m_storage(boost_ext::fusion::at_c<0>(s),
                    boost_ext::fusion::at_c<1>(s))
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class Sequence >
    struct operator_assign_enabler
        : sake::tuple_private::operator_assign_enabler<
              compressed_pair, Sequence >
    { };
public:
    template< class Sequence >
    typename operator_assign_enabler< SAKE_FWD_PARAM( Sequence ) >::type
        // -> compressed_pair&
    operator=(SAKE_FWD_REF( Sequence ) s)
    {
        first() = boost_ext::fusion::at_c<0>(sake::forward< Sequence >(s));
        second() = boost_ext::fusion::at_c<1>(sake::forward< Sequence >(s));
        return *this;
    }

    template< class U0, class U1 >
    void assign(SAKE_FWD_REF( U0 ) x0, SAKE_FWD_REF( U1 ) x1)
    {
        first() = sake::forward< U0 >(x0);
        second() = sake::forward< U1 >(x1);
    }

    typename boost_ext::add_reference< T0 >::type
    first()       { return m_storage.first(); }
    typename boost_ext::add_reference_add_const< T0 >::type
    first() const { return m_storage.first(); }

    typename boost_ext::add_reference< T1 >::type
    second()       { return m_storage.second(); }
    typename boost_ext::add_reference_add_const< T1 >::type
    second() const { return m_storage.second(); }

private:
    m_storage_type m_storage;
};

} // namespace compressed_pair_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_PAIR_COMPRESSED_PAIR_HPP
