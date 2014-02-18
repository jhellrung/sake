/*******************************************************************************
 * sake/core/operators_base/private/rv_sink.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_OPERATORS_BASE_PRIVATE_RV_SINK_HPP
#define SAKE_CORE_OPERATORS_BASE_PRIVATE_RV_SINK_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/has_qualifier.hpp>

namespace sake
{

namespace operators_base
{

namespace private_
{

template< class T >
struct rv_sink
{
    BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));

    T& value;

    template< class T_ >
    rv_sink(T_ const & value_,
        typename boost::enable_if_c<
            boost::is_same< T, T_ >::value >::type* = 0)
        : value(const_cast< T& >(value_))
    { }
};

} // namespace private_

} // namespace operators_base

} // namespace sake

#endif // #ifndef SAKE_CORE_OPERATORS_BASE_PRIVATE_RV_SINK_HPP
