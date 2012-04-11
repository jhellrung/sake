/*******************************************************************************
 * sake/core/functional/operators/shift_right.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::shift_right(T0&& x0, T1&& x1)
 *     -> operators::result_of::shift_right< T0, T1 >::type
 * struct operators::functional::shift_right
 *
 * struct operators::result_of::shift_right< T0, T1 >
 * struct operators::result_of::extension::shift_right0< T0, T1, Enable = void >
 * struct operators::result_of::extension::shift_right1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::shift_right< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_RIGHT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_RIGHT_HPP

#include <iosfwd>

#define SAKE_OPERATORS_NAME shift_right
#define SAKE_OPERATORS_OP   >>
#include <sake/core/functional/operators/private/binary.ipp>

namespace sake {
namespace operators {
namespace result_of {
namespace extension {
template< class T >
struct shift_right0< std::istream&, T >
{ typedef std::istream& type; };
} // namespace extension
} // namespace result_of
} // namespace operators
} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_RIGHT_HPP
