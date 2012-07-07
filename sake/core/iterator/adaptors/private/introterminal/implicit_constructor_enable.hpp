/*******************************************************************************
 * sake/core/iterator/adaptors/private/introterminal/implicit_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPLICIT_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPLICIT_CONSTRUCTOR_ENABLE_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/is_convertible.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

namespace introterminal_private
{

template< class Introterminal, class I >
struct implicit_constructor_enable_introterminal
    : boost_ext::is_convertible<
          typename sake::iterator_introterminal<I>::type, Introterminal >
{ };

template< class This, class I >
struct implicit_constructor_enable
    : boost_ext::mpl::and3<
          boost::mpl::not_< boost::is_same< This, I > >,
          sake::iterator::is_convertible<
              I const &, typename This::base_type >,
          implicit_constructor_enable_introterminal<
              typename This::iterator_introterminal, I >
      >
{ };

template< class This, class I, class T, class M >
struct implicit_constructor_enable<
    This, sake::iterator::adaptors::introterminal<I,T,M> >
{
    typedef sake::iterator::adaptors::introterminal<I,T,M> other_type;
    static bool const value = boost_ext::mpl::and3<
        boost::mpl::not_< boost::is_same< This, other_type > >,
        sake::iterator::is_convertible<
            I const &, typename This::base_type >,
        boost_ext::is_convertible<
            typename other_type::iterator_introterminal,
            typename This::iterator_introterminal
        >
    >::value;
    typedef implicit_constructor_enable type;
};

template< class This, class I >
struct implicit_constructor_enabler
    : boost::enable_if_c< implicit_constructor_enable< This, I >::value >
{ };

} // namespace introterminal_private

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_PRIVATE_INTROTERMINAL_IMPLICIT_CONSTRUCTOR_ENABLE_HPP
