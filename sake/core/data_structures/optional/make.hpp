/*******************************************************************************
 * sake/core/data_structures/optional/make.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * make_optional(T&& x) -> result_of::make_optional<T>::type
 ******************************************************************************/

#ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_MAKE_HPP
#define SAKE_CORE_DATA_STRUCTURES_OPTIONAL_MAKE_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/optional/optional.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/ref/wrapped_parameter_to_reference.hpp>

namespace sake
{

namespace result_of
{

template< class T >
struct make_optional
{
    typedef sake::optional<
        typename sake::wrapped_parameter_to_reference<
            typename boost_ext::remove_qualifiers<T>::type
        >::type
    > type;
};

} // namespace result_of

template< class T >
inline typename sake::result_of::make_optional<T>::type
make_optional(SAKE_FWD_REF( T ) x)
{
    typedef typename sake::result_of::make_optional<T>::type result_type;
    return result_type(sake::forward<T>(x));
}

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_OPTIONAL_MAKE_HPP
