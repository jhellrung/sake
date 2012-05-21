/*******************************************************************************
 * sake/core/emplacer/constructible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * emplacer_constructible<T>(U&& y) -> [unspecified]
 *
 * Returns an object (which may itself be a T object) from which a T object may
 * be constructed.  The intended application is constructor initializer lists.
 *
 * Note: This is overloaded for sake::emplacer arguments.
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_CONSTRUCTIBLE_HPP
#define SAKE_CORE_EMPLACER_CONSTRUCTIBLE_HPP

#include <boost/mpl/not.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

namespace sake
{

template< class T, class U >
inline SAKE_FWD2_REF( U )
emplacer_constructible(SAKE_FWD2_REF( U ) y)
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost::mpl::not_< boost_ext::is_reference<T> >,
        sake::is_convertible_wnrbt< SAKE_FWD2_PARAM( U ), T >
    >::value));
    return static_cast< SAKE_FWD2_REF( U ) >(y);
}

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_CONSTRUCTIBLE_HPP
