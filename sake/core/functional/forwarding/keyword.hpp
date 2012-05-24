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
 *     Type to typedef result_type to.  Must be included if either
 *     nullary_callable or nullary_const_callable are specified.
 * nullary_callable  [ optional ]
 *     Flag specifying to include an "operator()()" overload.
 * nullary_const_callable  [ optional ]
 *     Flag specifying to include an "operator()() const" overload.
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
SAKE_TEMPLATE_KEYWORD( nullary_callable )
SAKE_TEMPLATE_KEYWORD( nullary_const_callable )
SAKE_TEMPLATE_KEYWORD_VALUE( chained_base )

typedef boost::mpl::map0<> default_params;

} // namespace keyword

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_KEYWORD_HPP
