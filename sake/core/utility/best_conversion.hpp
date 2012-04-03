/*******************************************************************************
 * sake/core/utility/best_conversion.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct best_conversion< T, Sequence >
 *
 * This is a metafunction which evaluates to the type within the given
 * MPL.Sequence which is the best conversion from T, as determined by standard
 * overload resolution.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_BEST_CONVERSION_HPP
#define SAKE_CORE_UTILITY_BEST_CONVERSION_HPP

#include <boost/mpl/is_sequence.hpp>
#include <boost/static_assert.hpp>

#include <sake/core/expr_traits/best_conversion.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

template< class T, class Sequence >
struct best_conversion
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Sequence >::value));
    SAKE_EXPR_BEST_CONVERSION_TYPEDEF( typename sake::declval<T>(), Sequence, type );
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_BEST_CONVERSION_HPP
