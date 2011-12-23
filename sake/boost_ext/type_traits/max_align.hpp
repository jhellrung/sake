/*******************************************************************************
 * sake/boost_ext/type_traits/max_align.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * typedef ... boost_ext::max_align_t
 * std::size_t const boost_ext::max_align = ...
 *
 * max_align_t is a union which *should* have the maximum alignment requirement
 * on the compiler platform.
 *
 * (Largely borrowed from Boost.TypeTrait's type_with_alignment.)
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_MAX_ALIGN_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_MAX_ALIGN_HPP

#include <cstddef>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <sake/boost_ext/type_traits/types.hpp>

namespace sake
{

namespace boost_ext
{

union max_align_t
{
#define elem_i( r, data, i, elem ) elem BOOST_PP_CAT( _, i );
    BOOST_PP_SEQ_FOR_EACH_I( elem_i, ~, SAKE_BOOST_EXT_ALIGNMENT_TYPES )
#undef elem_i
};

std::size_t const max_align = boost::alignment_of< max_align_t >::value;

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_MAX_ALIGN_HPP
