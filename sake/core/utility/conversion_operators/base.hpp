/*******************************************************************************
 * sake/core/utility/conversion_operators/base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_BASE_HPP
#define SAKE_CORE_UTILITY_CONVERSION_OPERATORS_BASE_HPP

#include <boost/mpl/begin_end.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/conversion_operators/fwd.hpp>
#include <sake/core/utility/conversion_operators/private/iterate.hpp>

namespace sake
{

namespace conversion_operators
{

template<
  class Derived,
  class ConversionTypes,
  class ChainedBase /*= private_::empty*/
>
class base
  : public private_::iterate<
      Derived,
      typename boost::mpl::begin< ConversionTypes >::type,
      typename boost::mpl::end< ConversionTypes >::type,
      ChainedBase
    >
{
  typedef private_::iterate<
    Derived,
    typename boost::mpl::begin< ConversionTypes >::type,
    typename boost::mpl::end< ConversionTypes >::type,
    ChainedBase
  > iterate_;
public:
  SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
    typename base,
    (( iterate_ ))
  )
  SAKE_MEMBERWISE_SWAP(
    typename base,
    (( iterate_ ))
  )

protected:
  SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
    typename base,
    (( iterate_ ))
  )

  template< class T >
  explicit base(SAKE_FWD2_REF( T ) x,
    typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
      base, T >::value >::type* = 0)
    : iterate_(sake::forward<T>(x))
  { }

  template< class V >
  explicit base(sake::emplacer< V ( ) >)
  { }
};

} // namespace conversion_operators

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_BASE_HPP
