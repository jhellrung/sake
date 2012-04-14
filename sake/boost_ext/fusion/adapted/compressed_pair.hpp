/*******************************************************************************
 * sake/boost_ext/fusion/adapted/compressed_pair.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_PAIR_HPP
#define SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_PAIR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include <sake/core/utility/compressed_pair/fwd.hpp>

BOOST_FUSION_ADAPT_TPL_STRUCT(
    ( T0 ) ( T1 ),
    ( sake::compressed_pair ) ( T0 ) ( T1 ),
    ( T0, first() )
    ( T1, second() )
)

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_COMPRESSED_PAIR_HPP
