/*******************************************************************************
 * sake/core/range/basic/empty.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_EMPTY_HPP
#define SAKE_CORE_RANGE_BASIC_EMPTY_HPP

#include <boost/mpl/map/map10.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/has_qualifier.hpp>

#include <sake/core/iterator/basic/empty.hpp>
#include <sake/core/range/basic/fwd.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/facade.hpp>
#include <sake/core/range/keyword.hpp>
#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace basic
{

namespace empty_private
{

template< class T >
struct traits;

} // namespace empty_private

template< class T >
class empty
  : public empty_private::traits<T>::facade_
{
  typedef empty_private::traits<T> traits_;
  SAKE_USING_TYPEDEF( typename traits_, facade_ );
public:

  typedef boost::true_type has_trivial_default_constructor_tag;
  typedef boost::true_type has_trivial_copy_constructor_tag;
  typedef boost::true_type has_trivial_copy_assign_tag;
  typedef boost::true_type has_trivial_destructor_tag;

  SAKE_USING_TYPEDEF( typename facade_, iterator );
  SAKE_USING_TYPEDEF( typename facade_, reference );
  SAKE_USING_TYPEDEF( typename facade_, difference_type );
  SAKE_USING_TYPEDEF( typename facade_, size_type );

private:
  friend class sake::range::core_access;

  template< class This, class Introterminal >
  struct derived_iterator_with_of
  { typedef iterator type; };

  template< class This, class Begin, class End >
  struct derived_subrange_with_of
  { typedef empty type; };

  template< class This, class U, class Introterminal >
  static iterator
  derived_iter_at(This&, U, Introterminal)
  { return iterator(); }

  template< class This, class Begin, class End >
  static empty
  derived_sub(This&, Begin, End)
  { return empty(); }

  template< class This, class U >
  static reference
  derived_at(This&, U)
  {
    SAKE_REQUIRE((false));
    return *static_cast< T* >(0);
  }

  bool derived_empty() const
  { return true; }
  difference_type derived_distance() const
  { return static_cast< difference_type >(0); }
  size_type derived_size() const
  { return static_cast< size_type >(0); }
};

namespace empty_private
{

template< class T >
struct traits
{
  BOOST_STATIC_ASSERT((!boost_ext::has_qualifier<T>::value));
  typedef sake::range::facade<
    sake::range::basic::empty<T>,
    boost::mpl::map1<
      sake::range::keyword::iterator< sake::iterator::basic::empty<T> >
    >
  > facade_;
};

} // namespace empty_private

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_EMPTY_HPP
