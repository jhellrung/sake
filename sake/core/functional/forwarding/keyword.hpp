/*******************************************************************************
 * sake/core/functional/forwarding/keyword.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct forwarding::keyword::result< Value >
 * typedef ... forwarding::keyword::nullary_callable
 * typedef ... forwarding::keyword::nullary_const_callable
 * struct forwarding::keyword::chained_base< Value >
 *
 * This provides the keyword parameters for forwarding_base.
 *
 * result< Value >  [ optional ]
 *     Type to typedef result_type to.
 * nullary_callable< Value = default_tag >  [ optional ]
 *     Specifies to provide an "operator()()" overload and, optionally, the
 *     result type of this overload. If the result type is not specified with
 *     this keyword, it must be specified with the result keyword.
 * nullary_const_callable< Value = default_tag >  [ optional ]
 *     Specifies to provide an "operator()() const" overload and, optionally,
 *     the result type of this overload. If the result type is not specified
 *     with this keyword, it must be specified with the result keyword.
 * chained_base< Value >  [ optional ]
 *     Chained base class to inherit from.  Defaults to void_t.
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_KEYWORD_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_KEYWORD_HPP

#include <boost/mpl/map/map0.hpp>

#include <sake/core/utility/template_keyword.hpp>

namespace sake
{

namespace forwarding
{

namespace keyword
{

SAKE_TEMPLATE_KEYWORD_VALUE( result )
SAKE_TEMPLATE_KEYWORD_VALUE_DEFAULT( nullary_callable, void ( ) )
SAKE_TEMPLATE_KEYWORD_VALUE_DEFAULT( nullary_const_callable, void ( ) )
SAKE_TEMPLATE_KEYWORD_VALUE( chained_base )

typedef boost::mpl::map0<> default_params;

} // namespace keyword

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_KEYWORD_HPP
