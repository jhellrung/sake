/*******************************************************************************
 * sake/core/data_structures/pair/make.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * make_pair(T0&& x0, T1&& x1) -> result_of::make_pair< T0, T1 >::type
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_MAKE_HPP
#define SAKE_CORE_DATA_STRUCTURES_PAIR_MAKE_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/pair/pair.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>

namespace sake
{

namespace result_of
{

template< class T0, class T1 >
struct make_pair
{
    typedef sake::pair<
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T0 >::type
        >::type,
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers< T1 >::type
        >::type
    > type;
};

} // namespace result_of

template< class T0, class T1 >
inline typename result_of::make_pair< T0, T1 >::type
make_pair(SAKE_FWD_REF( T0 ) x0, SAKE_FWD_REF( T1 ) x1)
{
    typedef typename result_of::make_pair< T0, T1 >::type result_type;
    return result_type(sake::forward< T0 >(x0), sake::forward< T1 >(x1));
}

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_PAIR_MAKE_HPP
