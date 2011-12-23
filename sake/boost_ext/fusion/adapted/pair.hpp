/*******************************************************************************
 * sake/boost_ext/fusion/adapted/pair.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_PAIR_HPP
#define SAKE_BOOST_EXT_FUSION_ADAPTED_PAIR_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include <sake/core/data_structures/pair_fwd.hpp>

BOOST_FUSION_ADAPT_TPL_STRUCT(
    ( T0 ) ( T1 ),
    ( sake::pair ) ( T0 ) ( T1 ),
    ( T0, first )
    ( T1, second )
)

#endif // #ifndef SAKE_BOOST_EXT_FUSION_ADAPTED_PAIR_HPP
