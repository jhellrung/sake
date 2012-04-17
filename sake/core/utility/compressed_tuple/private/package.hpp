/*******************************************************************************
 * sake/core/utility/compressed_tuple/private/package.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_PACKAGE_HPP
#define SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_PACKAGE_HPP

#include <cstddef>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>

namespace sake
{

namespace compressed_tuple_adl
{

namespace private_
{

/*******************************************************************************
 * struct package< std::size_t I, std::size_t J >
 *
 * The apply static member function returns x0 if I < J, packages x0 and
 * x1 together if I == J, and returns x1 if I > J.
 ******************************************************************************/

template<
    std::size_t I, std::size_t J,
    bool = (I < J)
>
struct package;

template< std::size_t I, std::size_t J >
struct package< I, J, true >
{
    template< class T0, class T1 >
    static SAKE_FWD2_REF( T0 )
    apply(SAKE_FWD2_REF( T0 ) x0, T1 const &)
    { return sake::forward< T0 >(x0); }
};

template< std::size_t J >
struct package< J, J, false >
{
    template< class T0, class T1 >
    // Hack: const-qualified so it can bind to SAKE_FWD2_REF in C++03.
    static sake::emplacer< void ( SAKE_FWD2_PARAM( T0 ), SAKE_FWD2_PARAM( T1 ) ) > const
    apply(SAKE_FWD2_REF( T0 ) x0, SAKE_FWD2_REF( T1 ) x1)
    {
        typedef sake::emplacer< void ( SAKE_FWD2_PARAM( T0 ), SAKE_FWD2_PARAM( T1 ) ) > result_type;
        return result_type(sake::forward< T0 >(x0), sake::forward< T1 >(x1));
    }
};

template< std::size_t I, std::size_t J >
struct package< I, J, false >
{
    template< class T0, class T1 >
    static SAKE_FWD2_REF( T1 )
    apply(T0 const &, SAKE_FWD2_REF( T1 ) x1)
    { return sake::forward< T1 >(x1); }
};

} // namespace private_

} // namespace compressed_tuple_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_COMPRESSED_TUPLE_PRIVATE_PACKAGE_HPP
