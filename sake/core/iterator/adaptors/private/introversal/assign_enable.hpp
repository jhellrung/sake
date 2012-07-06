/*******************************************************************************
 * sake/core/iterator/adaptors/private/introversal/assign_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_ASSIGN_ENABLE_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_ASSIGN_ENABLE_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace introversal_private
{

template< class Introversal, class I >
struct assign_enable_introversal
    : boost_ext::is_convertible<
          typename sake::iterator_introversal<I>::type,
          Introversal
      >
{ };

template< class This, class I >
struct assign_enable
    : boost_ext::mpl::and3<
          boost::mpl::not_< boost::is_same< This, I > >,
          sake::has_operator_assign< typename This::base_type &, I const & >,
          assign_enable_introversal< typename This::iterator_introversal, I >
      >
{ };

template< class This, class I, class T, class M >
struct assign_enable< This, sake::iterator::adaptors::introversal<I,T,M> >
{
    typedef sake::iterator::adaptors::introversal<I,T,M> other_type;
    static bool const value = boost_ext::mpl::and3<
        boost::mpl::not_< boost::is_same< This, other_type > >,
        sake::has_operator_assign< typename This::base_type &, I const & >,
        boost_ext::is_convertible<
            typename other_type::iterator_introversal,
            typename This::iterator_introversal
        >
    >::value;
    typedef assign_enable type;
};

template< class This, class I >
struct assign_enabler
    : boost::enable_if_c< assign_enable< This, I >::value, This& >
{ };

} // namespace introversal_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROVERSAL_ASSIGN_ENABLE_HPP
