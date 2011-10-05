/*******************************************************************************
 * sake/core/functional/forwarding/deduced_enable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct forwarding::deduced_enable< Signature, T >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_ENABLE_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_ENABLE_HPP

#include <boost/function_types/result_type.hpp>

#include <sake/boost_ext/function_types/replace_result_type.hpp>
#include <sake/boost_ext/type_traits/propagate_cv.hpp>

#include <sake/core/introspection/is_callable.hpp>

namespace sake
{

namespace forwarding
{

template< class Signature, class T >
struct deduced_enable
    : sake::is_callable<
          typename boost_ext::propagate_cv<
              typename boost::function_types::result_type< Signature >::type,
              T
          >::type,
          typename boost_ext::function_types::replace_result_type< Signature, void >::type
      >
{ };

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_ENABLE_HPP
