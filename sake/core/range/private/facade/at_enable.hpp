/*******************************************************************************
 * sake/core/range/private/facade/at_enable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_AT_ENABLE_HPP
#define SAKE_CORE_RANGE_PRIVATE_FACADE_AT_ENABLE_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/private/facade/traits.hpp>

namespace sake
{

namespace range
{

namespace facade_adl
{

namespace private_
{

template< class P, class T >
struct at_enable
{
    typedef private_::traits<P> traits_;
    static bool const value = boost_ext::mpl::or3<
        boost::is_same< T, sake::begin_tag >,
        boost_ext::mpl::and2<
            boost::is_same< T, sake::end_tag >,
            typename traits_::back_enable
        >,
        boost_ext::mpl::and2<
            boost_ext::is_convertible< T, typename traits_::size_type >,
            boost_ext::is_convertible<
                typename traits_::range_traversal,
                boost::random_access_traversal_tag
            >
        >
    >::value;
    typedef at_enable type;
};

template< class P, class T, class R = void >
struct at_enabler
    : boost::enable_if_c< private_::at_enable<P,T>::value, R >
{ };

} // namespace private_

} // namespace facade_adl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_FACADE_AT_ENABLE_HPP
