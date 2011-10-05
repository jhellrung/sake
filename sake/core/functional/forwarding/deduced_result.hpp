/*******************************************************************************
 * sake/core/functional/forwarding/deduced_result.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct forwarding::deduced_result< Signature, T >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_RESULT_HPP
#define SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_RESULT_HPP

#include <boost/function_types/result_type.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/boost_ext/function_types/replace_result_type.hpp>
#include <sake/boost_ext/type_traits/propagate_cv.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

namespace sake
{

namespace forwarding
{

template< class Signature, class T >
struct deduced_result
    : boost::result_of<
          typename boost_ext::function_types::replace_result_type<
              Signature,
              typename boost_ext::remove_reference<
                  typename boost_ext::propagate_cv<
                      typename boost::function_types::result_type< Signature >::type,
                      T
                  >::type
              >::type
          >::type
      >
{ };

} // namespace forwarding

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_FORWARDING_DEDUCED_RESULT_HPP
