/*******************************************************************************
 * sake/core/utility/nullptr.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct nullptr_t
 * nullptr_t const nullptr
 *
 * operator==(nullptr_t, nullptr_t) -> bool
 * operator!=(nullptr_t, nullptr_t) -> bool
 *
 * operator<<(std::basic_ostream< T, Traits > & o, nullptr_t)
 *   -> std::basic_ostream< T, Traits > &
 *
 * See
 *     http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr
 * This provides a replacement of the nullptr keyword (from C++11) in C++03.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NULLPTR_HPP
#define SAKE_CORE_UTILITY_NULLPTR_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_NULLPTR

#include <cstddef>

namespace sake
{

typedef std::nullptr_t nullptr_t;

} // namespace sake

#else // #ifndef BOOST_NO_NULLPTR

#include <iosfwd>

namespace sake
{

struct nullptr_t
{
  template< class T >
  operator T * () const
  { return static_cast< T * >(0); }

  template< class C, class T >
  operator T C::* () const
  { return static_cast< T C::* >(0); }

#ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

  explicit operator bool () { return false; }

#else // #ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

private:
  typedef int (nullptr_t::*_sake_explicit_operator_bool_result_type);
public:
  operator _sake_explicit_operator_bool_result_type () const { return 0; }

#endif // #ifndef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS

  // Must be public in order for nullptr_t to be POD.
  void* _sake_nullptr_sizeof;

#ifndef BOOST_NO_DELETED_FUNCTIONS
  void operator & () const = delete;
#else // #ifndef BOOST_NO_DELETED_FUNCTIONS
  private: void operator & () const;
#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS
};

sake::nullptr_t const nullptr = { 0 };

inline bool operator==(sake::nullptr_t, sake::nullptr_t) { return true; }
inline bool operator!=(sake::nullptr_t, sake::nullptr_t) { return false; }

template< class T, class Traits >
inline std::basic_ostream< T, Traits > &
operator<<(std::basic_ostream< T, Traits > & o, sake::nullptr_t)
{ return o << static_cast< void * >(0); }

} // namespace sake

using sake::nullptr;

#endif // #ifndef BOOST_NO_NULLPTR

#endif // #ifndef SAKE_CORE_UTILITY_NULLPTR_HPP
