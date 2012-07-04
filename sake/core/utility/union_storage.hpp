/*******************************************************************************
 * sake/core/utility/union_storage.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct union_storage< Sequence >
 *
 * union_storage< Sequence >::type is a union type of the proper alignment and
 * size to store any type within the given Boost.MPL Sequence.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_UNION_STORAGE_HPP
#define SAKE_CORE_UTILITY_UNION_STORAGE_HPP

#include <cstddef>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/lcm.hpp>
#include <sake/boost_ext/type_traits/aligned_storage.hpp>

namespace sake
{

template< class Sequence >
struct union_storage
{
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< Sequence >::value));

    static std::size_t const size = boost::mpl::fold<
        Sequence,
        boost::integral_constant< std::size_t, 0 >,
        boost::mpl::max< boost::mpl::_1, boost::mpl::sizeof_< boost::mpl::_2 > >
    >::type::value;

    static std::size_t const align = boost::mpl::fold<
        Sequence,
        boost::integral_constant< std::size_t, 1 >,
        boost_ext::mpl::lcm< boost::mpl::_1, boost::alignment_of< boost::mpl::_2 > >
    >::type::value;

    typedef typename boost_ext::aligned_storage_c< size, align >::type type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_UNION_STORAGE_HPP
