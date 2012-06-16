/*******************************************************************************
 * sake/core/iterator/private/introversal_adaptor/implicit_constructor_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_IMPLICIT_CONSTRUCTOR_ENABLE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_IMPLICIT_CONSTRUCTOR_ENABLE_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_convertible_wndp2bp.hpp>

#include <sake/core/iterator/introversal_adaptor_fwd.hpp>
#include <sake/core/iterator/traits.hpp>

namespace sake
{

namespace iterator
{

namespace introversal_adaptor_private
{

template< class Introversal, class I >
struct implicit_constructor_enable_introversal
    : boost_ext::is_convertible<
          typename sake::iterator_introversal<I>::type, Introversal >
{ };

template< class This, class I >
struct implicit_constructor_enable
    : boost_ext::mpl::and3<
          boost::mpl::not_< boost::is_same< This, I > >,
          boost_ext::is_convertible_wndp2bp<
              I const &, typename This::base_type >,
          implicit_constructor_enable_introversal<
              typename This::iterator_introversal, I >
      >
{ };

template< class This, class I, bool B, bool E, class M >
struct implicit_constructor_enable<
    This, sake::iterator::introversal_adaptor<I,B,E,M> >
{
    typedef sake::iterator::introversal_adaptor<I,B,E,M> introversal_adaptor_;
    static bool const value = boost_ext::mpl::and3<
        boost::mpl::not_< boost::is_same< This, introversal_adaptor_ > >,
        boost_ext::is_convertible_wndp2bp<
            I const &, typename This::base_type >,
        boost_ext::is_convertible<
            typename introversal_adaptor_::iterator_introversal,
            typename This::iterator_introversal
        >
    >::value;
    typedef implicit_constructor_enable type;
};

template< class This, class I >
struct implicit_constructor_enabler
    : boost::enable_if_c< implicit_constructor_enable< This, I >::value >
{ };

} // namespace introversal_adaptor_private

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_INTROVERSAL_ADAPTOR_IMPLICIT_CONSTRUCTOR_ENABLE_HPP
