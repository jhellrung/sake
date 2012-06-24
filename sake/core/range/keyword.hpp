/*******************************************************************************
 * sake/core/range/keyword.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::keyword::iterator<T>
 * struct range::keyword::const_iterator<T>
 *
 * struct range::keyword::back_enable<T>
 * struct range::keyword::back_enable_c< Cond >
 * struct range::keyword::size_enable<T>
 * struct range::keyword::size_enable_c< Cond >
 *
 * struct range::keyword::chained_base<T>
 * struct range::keyword::member<T>
 *
 * ... range::keyword::_base
 * ... range::keyword::_member
 * ... range::keyword::_chained_base
 *
 * This provides the keywords for range::facade and range::adaptor.
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_KEYWORD_HPP
#define SAKE_CORE_RANGE_KEYWORD_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/keyword/keyword.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace range
{

namespace keyword
{

SAKE_TEMPLATE_KEYWORD_VALUE( iterator )
SAKE_TEMPLATE_KEYWORD_VALUE( const_iterator )

SAKE_TEMPLATE_KEYWORD_VALUE( chained_base )
SAKE_TEMPLATE_KEYWORD_VALUE( member )

SAKE_TEMPLATE_KEYWORD_VALUE( back_enable )
template< bool Cond >
struct back_enable_c
    : sake::range::keyword::back_enable<
          boost::integral_constant< bool, Cond > >
{ };

SAKE_TEMPLATE_KEYWORD_VALUE( size_enable )
template< bool Cond >
struct size_enable_c
    : sake::range::keyword::size_enable<
          boost::integral_constant< bool, Cond > >
{ };

SAKE_KEYWORD( base )
SAKE_KEYWORD( chained_base )
SAKE_KEYWORD( member )

} // namespace keyword

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_KEYWORD_HPP
