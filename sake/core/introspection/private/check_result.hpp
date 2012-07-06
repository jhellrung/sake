/*******************************************************************************
 * sake/core/introspection/private/check_result.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_CHECK_RESULT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_CHECK_RESULT_HPP

#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/private/dummy.hpp>

namespace sake
{

namespace introspection_private
{

template< class Result, class ResultPred >
struct check_result
{
    template< class T >
    struct apply
        : boost_ext::mpl::and3<
              boost::mpl::not_< boost::is_same<
                  T, introspection_private::dummy > >,
              boost_ext::is_convertible< T, Result >,
              boost::mpl::apply1< ResultPred, T >
          >
    { };
};

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_CHECK_RESULT_HPP
