/*******************************************************************************
 * sake/core/keyword/param_spec.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_PARAM_SPEC_HPP
#define SAKE_CORE_KEYWORD_PARAM_SPEC_HPP

#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

namespace keyword
{

template< class Tag >
struct deduced;

template< class Tag, class Pred = void >
struct optional;

template< class Tag, class Pred = void >
struct required;

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_PARAM_SPEC_HPP
