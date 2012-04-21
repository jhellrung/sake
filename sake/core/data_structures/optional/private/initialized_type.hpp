/*******************************************************************************
 * sake/core/data_structures/optional/private/initialized_type.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_INITIALIZED_TYPE_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_INITIALIZED_TYPE_HPP

#include <cstddef>

#include <boost/type_traits/alignment_of.hpp>

namespace sake
{

namespace optional_adl
{

namespace private_
{

std::size_t const alignment_of_bool = boost::alignment_of< bool >::value;

template<
    class T,
    bool = (alignment_of_bool > boost::alignment_of<T>::value)
>
struct initialized_type;

template< class T >
struct initialized_type< T, false >
{ typedef bool type; };

template< class T >
struct initialized_type< T, true >
{ typedef unsigned char type; };

} // namespace private_

} // namespace optional_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_PRIVATE_INITIALIZED_TYPE_HPP
