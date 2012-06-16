/*******************************************************************************
 * sake/core/iterator/keyword.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_keyword::value<T>
 * struct iterator_keyword::reference<T>
 * struct iterator_keyword::difference<T>
 * struct iterator_keyword::traversal<T>
 * struct iterator_keyword::introversal<T>
 *
 * struct iterator_keyword::compare_enable<T>
 * struct iterator_keyword::compare_enable_c< Cond >
 * struct iterator_keyword::difference_enable<T>
 * struct iterator_keyword::difference_enable_c< Cond >
 *
 * struct iterator_keyword::chained_base<T>
 * struct iterator_keyword::member<T>
 *
 * ... iterator_keyword::_base
 * ... iterator_keyword::_member
 * ... iterator_keyword::_chained_base
 *
 * typedef ... iterator_keyword::incrementable_traversal
 * typedef ... iterator_keyword::single_pass_traversal
 * typedef ... iterator_keyword::forward_traversal
 * typedef ... iterator_keyword::bidirectional_traversal
 * typedef ... iterator_keyword::random_access_traversal
 *
 * This provides the keywords for iterator_facade and iterator_adaptor.
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_KEYWORD_HPP
#define SAKE_CORE_ITERATOR_KEYWORD_HPP

#include <boost/iterator/iterator_categories.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/keyword/keyword.hpp>
#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace iterator_keyword
{

SAKE_TEMPLATE_KEYWORD_VALUE( value )
SAKE_TEMPLATE_KEYWORD_VALUE( reference )
SAKE_TEMPLATE_KEYWORD_VALUE( difference )
SAKE_TEMPLATE_KEYWORD_VALUE( traversal )
SAKE_TEMPLATE_KEYWORD_VALUE( introversal )

SAKE_TEMPLATE_KEYWORD_VALUE( chained_base )
SAKE_TEMPLATE_KEYWORD_VALUE( member )

SAKE_TEMPLATE_KEYWORD_VALUE( compare_enable )
template< bool Cond >
struct compare_enable_c
    : sake::iterator_keyword::compare_enable<
          boost::integral_constant< bool, Cond > >
{ };

SAKE_TEMPLATE_KEYWORD_VALUE( difference_enable )
template< bool Cond >
struct difference_enable_c
    : sake::iterator_keyword::difference_enable<
          boost::integral_constant< bool, Cond > >
{ };

SAKE_KEYWORD( base )
SAKE_KEYWORD( chained_base )
SAKE_KEYWORD( member )

#define typedef_traversal( name ) \
typedef sake::iterator_keyword::traversal< \
    boost::name ## _traversal_tag > name ## _traversal;
typedef_traversal( incrementable )
typedef_traversal( single_pass )
typedef_traversal( forward )
typedef_traversal( bidirectional )
typedef_traversal( random_access )
#undef typedef_traversal

} // namespace iterator_keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_KEYWORD_HPP