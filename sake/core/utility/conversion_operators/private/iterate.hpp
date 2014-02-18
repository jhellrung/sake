/*******************************************************************************
 * sake/core/utility/conversion_operators/private/iterate.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_PRIVATE_ITERATE_HPP
#define SAKE_CORE_UTILITY_CONVERSION_OPERATORS_PRIVATE_ITERATE_HPP

#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>

#include <sake/core/emplacer/constructible.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/utility/conversion_operators/core_access.hpp>
#include <sake/core/utility/conversion_operators/fwd.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace conversion_operators
{

namespace private_
{

template< class Derived, class I, class E, class ChainedBase >
class iterate
  : public private_::iterate<
      Derived, typename boost::mpl::next<I>::type, E, ChainedBase >
{
  typedef private_::iterate<
    Derived, typename boost::mpl::next<I>::type, E, ChainedBase > iterate_;
protected:
  using iterate_::derived;
public:
  SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
    typename iterate,
    (( iterate_ ))
  )
  SAKE_MEMBERWISE_SWAP(
    typename iterate,
    (( iterate_ ))
  )

private:
  typedef typename boost::mpl::deref<I>::type conversion_type;
public:
  operator conversion_type()
  {
    return sake::conversion_operators::core_access::convert(
      derived(), sake::type_tag< conversion_type >());
  }
  operator conversion_type() const
  {
    return sake::conversion_operators::core_access::convert(
      derived(), sake::type_tag< conversion_type >());
  }

protected:
  SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
    typename iterate,
    (( iterate_ ))
  )

  template< class T >
  explicit iterate(SAKE_FWD2_REF( T ) x,
    typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
      iterate, T >::value >::type * = 0)
    : iterate_(sake::forward<T>(x))
  { }
};

template< class Derived, class E, class ChainedBase >
class iterate< Derived, E, E, ChainedBase >
  : public ChainedBase
{
protected:
  typedef ChainedBase chained_base_type;
public:
  SAKE_BASIC_MOVABLE_COPYABLE_MEMBERWISE(
    typename iterate,
    (( chained_base_type ))
  )
  SAKE_MEMBERWISE_SWAP(
    typename iterate,
    (( chained_base_type ))
  )
protected:
  Derived       & derived()
  { return *static_cast< Derived       * >(this); }
  Derived const & derived() const
  { return *static_cast< Derived const * >(this); }

  SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
    typename iterate,
    (( chained_base_type ))
  )

  template< class T >
  explicit iterate(SAKE_FWD2_REF( T ) x,
    typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
      iterate, T >::value >::type * = 0)
    : chained_base_type(sake::emplacer_constructible<
        chained_base_type >(sake::forward<T>(x)))
  { }
};

} // namespace private_

} // namespace conversion_operators

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_CONVERSION_OPERATORS_PRIVATE_ITERATE_HPP
