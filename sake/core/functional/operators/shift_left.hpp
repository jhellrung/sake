/*******************************************************************************
 * sake/core/functional/operators/shift_left.hpp
 *
 * Copyleft 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * operators::shift_left(T0&& x0, T1&& x1)
 *     -> operators::result_of::shift_left< T0, T1 >::type
 * struct operators::functional::shift_left
 *
 * struct operators::result_of::shift_left< T0, T1 >
 * struct operators::result_of::extension::shift_left0< T0, T1, Enable = void >
 * struct operators::result_of::extension::shift_left1< T0, T1, Enable = void >
 * struct operators::result_of::default_impl::shift_left< T0, T1 >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_HPP
#define SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_HPP

#include <iosfwd>

#define SAKE_OPERATORS_NAME shift_left
#define SAKE_OPERATORS_OP   <<
#include <sake/core/functional/operators/private/binary.ipp>

namespace sake {
namespace operators {
namespace result_of {
namespace extension {
template< class T >
struct shift_left0< std::ostream&, T >
{ typedef std::ostream& type; };
} // namespace extension
} // namespace result_of
} // namespace operators
} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_OPERATORS_SHIFT_LEFT_HPP
