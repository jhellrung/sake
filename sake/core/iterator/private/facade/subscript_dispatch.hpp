/*******************************************************************************
 * sake/core/iterator/private/facade/subscript_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_SUBSCRIPT_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_SUBSCRIPT_DISPATCH_HPP

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

namespace private_
{

/*******************************************************************************
 * operator[] must return a proxy in case iterator destruction invalidates
 * referents.
 * To see why, consider the following implementation of operator[]:
 *   reference operator[](difference_type n) const
 *   { return *(*this + n); }
 * The problem here is that operator[] would return a reference created from
 * a temporary iterator.
 ******************************************************************************/

template< class Value, class Reference >
struct subscript_dispatch_bool
{ static bool const value = false; };

template< class Value, class Reference >
struct subscript_dispatch_bool< Value const, Reference >
  : boost_ext::mpl::and2<
      boost_ext::mpl::or2<
        boost::has_trivial_copy< Value >,
        sake::is_by_value_optimal< Value >
      >,
      boost_ext::is_convertible< Value, Reference >
    >
{ };

template<
  class This, class Value, class Reference,
  bool = subscript_dispatch_bool< Value, Reference >::value
>
struct subscript_dispatch;

template< class This, class Value, class Reference >
struct subscript_dispatch< This, Value const, Reference, true >
{
  typedef Value type;
  static type apply(This const & this_)
  { return *this_; }
};

template< class This, class Value, class Reference >
struct subscript_dispatch< This, Value const, Reference, false >
{
  class proxy
  {
    This const m_this;
    explicit proxy(This const & this_) : m_this(this_) { }
    friend struct subscript_dispatch;
  public:
    SAKE_NONCOPY_ASSIGNABLE( proxy )
    operator Reference() const { return *m_this; }
  };

  typedef proxy type;
  static type apply(This const & this_)
  { return proxy(this_); }
};

template< class This, class Value, class Reference >
struct subscript_dispatch< This, Value, Reference, false >
{
  class proxy
  {
    This const m_this;
    explicit proxy(This const & this_) : m_this(this_) { }
    friend struct subscript_dispatch;
  public:
    SAKE_NONCOPY_ASSIGNABLE( proxy )
    operator Reference() const { return *m_this; }

  private:
    template< class T >
    struct operator_assign_enable
      : sake::has_operator_assign< Reference, void ( T ) >
    { };
    template< class T >
    struct operator_assign_enabler
      : boost::enable_if_c<
          operator_assign_enable<T>::value, proxy const & >
    { };
  public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class T >
    typename operator_assign_enabler<T>::type
    operator=(T&& x) const
    { *m_this = sake::forward<T>(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

  private:
    typedef sake::rv_sink_traits1<
      Value, boost::mpl::quote1< operator_assign_enable >
    > operator_assign_rv_sink_traits;
    typedef typename operator_assign_rv_sink_traits::template
      default_< sake::rv_sink_visitors::operator_assign< proxy const > >
      operator_assign_rv_sink_default_type;
  public:
    // lvalues + movable explicit rvalues
    template< class T >
    typename operator_assign_rv_sink_traits::template
      ref_enabler< T, proxy const & >::type
    operator=(T& x) const
    { *m_this = x; return *this; }
    // Value rvalues
    proxy const &
    operator=(typename operator_assign_rv_sink_traits::primary_type x) const
    { *m_this = sake::move(x.value); return *this; }
    // movable implicit rvalues
    proxy const &
    operator=(operator_assign_rv_sink_default_type x) const
    { return x(sake::rv_sink_visitors::construct::operator_assign(*this)); }
    // const lvalues + non-movable rvalues
    template< class T >
    typename operator_assign_rv_sink_traits::template
      cref_enabler< T, proxy const & >::type
    operator=(T const & x) const
    { *m_this = x; return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

  };

  typedef proxy type;
  static type apply(This const & this_)
  { return proxy(this_); }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_SUBSCRIPT_DISPATCH_HPP
