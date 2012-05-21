/*******************************************************************************
 * sake/core/emplacer/private/cast.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_PRIVATE_CAST_HPP
#define SAKE_CORE_EMPLACER_PRIVATE_CAST_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>

#include <sake/core/utility/is_by_value_optimal.hpp>

namespace sake
{

namespace emplacer_private
{

template< class U, bool = sake::is_by_value_optimal<U>::value >
struct cast;

template< class U >
struct cast< U, false >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<U>::value));
    typedef typename boost_ext::add_reference_add_const<
        typename boost_ext::add_rvalue_reference<U>::type
    >::type type;
    template< class V >
    static type apply(V& x)
    { return static_cast< type >(x); }
};

template< class U >
struct cast< U, true >
{
    BOOST_STATIC_ASSERT((!boost_ext::is_rvalue_reference<U>::value));
    typedef typename boost::remove_cv<U>::type type;
    static type apply(type x)
    { return x; }
};

} // namespace emplacer_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_PRIVATE_CAST_HPP
