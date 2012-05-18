/*******************************************************************************
 * sake/core/utility/nullptr.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_USING_NULLPTR
 *
 * struct nullptr_t
 * nullptr_t const nullptr
 *
 * operator==(nullptr_t, nullptr_t) -> bool
 * operator!=(nullptr_t, nullptr_t) -> bool
 * operator==(nullptr_t, T* p) -> bool
 * operator!=(nullptr_t, T* p) -> bool
 * operator==(T* p, nullptr_t) -> bool
 * operator!=(T* p, nullptr_t) -> bool
 *
 * operator<<(std::ostream& out, nullptr_t) -> std::ostream&
 *
 * See
 *     http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr
 * This provides a replacement of the nullptr keyword (from C++11) in C++03.
 * One should always use nullptr unqualified.  The macro SAKE_USING_NULLPTR
 * should be used when outside the sake namespace to bring nullptr into the
 * current scope.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_NULLPTR_HPP
#define SAKE_CORE_UTILITY_NULLPTR_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_NULLPTR

#define SAKE_USING_NULLPTR static_cast<void>(0)

namespace sake
{

typedef std::nullptr_t nullptr_t;

} // namespace sake

#else // #ifndef BOOST_NO_NULLPTR

#include <ostream>

#include <boost/static_assert.hpp>

#include <sake/core/utility/explicit_operator_bool.hpp>

#define SAKE_USING_NULLPTR using ::sake::nullptr

namespace sake
{

struct nullptr_t
{
    template< class T >
    operator T*() const
    { return static_cast< T* >(0); }

    template< class C, class T >
    operator T C::*() const
    { return static_cast< T C::* >(0); }

    SAKE_EXPLICIT_OPERATOR_BOOL() const
    { return false; }

    // Must be public in order for nullptr_t to be POD.
    void* _sake_nullptr_sizeof;

#ifndef BOOST_NO_DELETED_FUNCTIONS
    void operator&() const = delete;
#else // #ifndef BOOST_NO_DELETED_FUNCTIONS
    private: void operator&() const;
#endif // #ifndef BOOST_NO_DELETED_FUNCTIONS
};

BOOST_STATIC_ASSERT( sizeof( sake::nullptr_t ) == sizeof( void* ) );

sake::nullptr_t const nullptr = { 0 };

inline bool operator==(sake::nullptr_t, sake::nullptr_t) { return true; }
inline bool operator!=(sake::nullptr_t, sake::nullptr_t) { return false; }

template< class T > inline bool operator==(sake::nullptr_t, T* const p) { return p == 0; }
template< class T > inline bool operator!=(sake::nullptr_t, T* const p) { return p != 0; }
template< class T > inline bool operator==(T* const p, sake::nullptr_t) { return p == 0; }
template< class T > inline bool operator!=(T* const p, sake::nullptr_t) { return p != 0; }

inline std::ostream&
operator<<(std::ostream& out, sake::nullptr_t)
{ return out << static_cast< void* >(0); }

namespace nullptr_private
{

namespace
{

template< class T > int test(T);
BOOST_STATIC_ASSERT( sizeof( test< void* >(nullptr) ) );
BOOST_STATIC_ASSERT( sizeof( test< int* >(nullptr) ) );
BOOST_STATIC_ASSERT( sizeof( test< bool >(nullptr) ) );

} // namespace

} // namespace nullptr_private

} // namespace sake

#endif // #ifndef BOOST_NO_NULLPTR

#endif // #ifndef SAKE_CORE_UTILITY_NULLPTR_HPP
