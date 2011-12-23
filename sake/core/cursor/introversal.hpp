/*******************************************************************************
 * sake/core/cursor/introversal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct cursor_introversal<C>
 * struct extension::cursor_introversal< C, Enable = void >
 * struct default_impl::cursor_introversal<C>
 *
 * struct cursor_begin_introversal<C>
 * struct cursor_end_introversal<C>
 *
 * struct cursor_introversal_join< C0, C1 > // "or"
 * struct cursor_introversal_meet< C0, C1 > // "and"
 *
 * struct cursor_introversal_tag_c< Value >
 * typedef ... null_introversal_tag
 * typedef ... begin_detect_introversal_tag
 * typedef ... begin_access_introversal_tag
 * typedef ... end_detect_introversal_tag
 * typedef ... end_access_introversal_tag
 * typedef ... begin_detect_end_detect_introversal_tag
 * typedef ... begin_access_end_detect_introversal_tag
 * typedef ... begin_detect_end_access_introversal_tag
 * typedef ... begin_access_end_access_introversal_tag
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_INTROVERSAL_HPP
#define SAKE_CORE_CURSOR_INTROVERSAL_HPP

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/introversal_fwd.hpp>
#include <sake/core/introspection/has_type.hpp>

namespace sake
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_cursor_introversal, cursor_introversal )

namespace cursor_introversal_tag_values
{

unsigned int const null = 0;
unsigned int const begin_detect = 1;
unsigned int const begin_access = 1|2;
unsigned int const end_detect   = 4;
unsigned int const end_access   = 4|8;
unsigned int const begin_detect_end_detect = begin_detect | end_detect;
unsigned int const begin_access_end_detect = begin_access | end_detect;
unsigned int const begin_detect_end_access = begin_detect | end_access;
unsigned int const begin_access_end_access = begin_access | end_access;

} // namespace cursor_introversal_tag_values

/*******************************************************************************
 * struct cursor_introversal_tag_c< Value >
 * typedef ... null_introversal_tag
 * typedef ... begin_detect_introversal_tag
 * typedef ... begin_access_introversal_tag
 * typedef ... end_detect_introversal_tag
 * typedef ... end_access_introversal_tag
 * typedef ... begin_detect_end_detect_introversal_tag
 * typedef ... begin_access_end_detect_introversal_tag
 * typedef ... begin_detect_end_access_introversal_tag
 * typedef ... begin_access_end_access_introversal_tag
 ******************************************************************************/

template< unsigned int Value >
struct cursor_introversal_tag_c;

typedef cursor_introversal_tag_c< cursor_introversal_tag_values::null > null_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect > begin_detect_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access > begin_access_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::end_detect > end_detect_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::end_access > end_access_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect_end_detect > begin_detect_end_detect_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access_end_detect > begin_access_end_detect_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect_end_access > begin_detect_end_access_introversal_tag;
typedef cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access_end_access > begin_access_end_access_introversal_tag;

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::null >
{ static unsigned int const value = cursor_introversal_tag_values::null; };

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect >
    : null_introversal_tag
{ static unsigned int const value = cursor_introversal_tag_values::begin_detect; };

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access >
    : begin_detect_introversal_tag
{ static unsigned int const value = cursor_introversal_tag_values::begin_access; };

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::end_detect >
    : null_introversal_tag
{ static unsigned int const value = cursor_introversal_tag_values::end_detect; };

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::end_access >
    : end_detect_introversal_tag
{ static unsigned int const value = cursor_introversal_tag_values::end_access; };

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect_end_detect >
    : null_introversal_tag
{
    static unsigned int const value = cursor_introversal_tag_values::begin_detect_end_detect;
    operator begin_detect_introversal_tag() const { return begin_detect_introversal_tag(); }
    operator end_detect_introversal_tag() const { return end_detect_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access_end_detect >
    : begin_detect_end_detect_introversal_tag
{
    static unsigned int const value = cursor_introversal_tag_values::begin_access_end_detect;
    operator begin_access_introversal_tag() const { return begin_access_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_detect_end_access >
    : begin_detect_end_detect_introversal_tag
{
    static unsigned int const value = cursor_introversal_tag_values::begin_detect_end_access;
    operator end_access_introversal_tag() const { return end_access_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c< cursor_introversal_tag_values::begin_access_end_access >
    : begin_detect_end_detect_introversal_tag
{
    static unsigned int const value = cursor_introversal_tag_values::begin_access_end_access;
    operator begin_access_introversal_tag() const { return begin_access_introversal_tag(); }
    operator end_access_introversal_tag() const { return end_access_introversal_tag(); }
    operator begin_access_end_detect_introversal_tag() const { return begin_access_end_detect_introversal_tag(); }
    operator begin_detect_end_access_introversal_tag() const { return begin_detect_end_access_introversal_tag(); }
};

/*******************************************************************************
 * struct cursor_introversal_join< C0, C1 > // "or"
 * struct cursor_introversal_meet< C0, C1 > // "and"
 ******************************************************************************/

template< class C0, class C1 >
struct cursor_introversal_join
{ typedef cursor_introversal_tag_c< C0::value | C1::value > type; };

template< class C0, class C1 >
struct cursor_introversal_meet
{ typedef cursor_introversal_tag_c< C0::value & C1::value > type; };

/*******************************************************************************
 * struct cursor_introversal<C>
 ******************************************************************************/

template< class C >
struct cursor_introversal
{
    typedef typename extension::cursor_introversal<C>::type type;
    static unsigned int const value = type::value;
};

/*******************************************************************************
 * struct extension::cursor_introversal< C, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class C, class Enable /*= void*/ >
struct cursor_introversal
    : default_impl::cursor_introversal<C>
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::cursor_introversal<C>
 ******************************************************************************/

namespace default_impl
{

namespace cursor_introversal_private
{

template< class C, bool = sake::has_type_cursor_introversal<C>::value >
struct dispatch;

template< class C >
struct dispatch< C, false >
{ typedef sake::null_introversal_tag type; };

template< class C >
struct dispatch< C, true >
{ typedef typename C::cursor_introversal type; };

} // namespace cursor_introversal_private

template< class C >
struct cursor_introversal
    : cursor_introversal_private::dispatch<C>
{ };

} // namespace default_impl

/*******************************************************************************
 * struct cursor_begin_introversal<C>
 * struct cursor_end_introversal<C>
 ******************************************************************************/

template< class C >
struct cursor_begin_introversal
{
    typedef sake::cursor_introversal_tag_c<
        sake::cursor_introversal<C>::value &
        sake::begin_access_introversal_tag::value
    > type;
    static unsigned int const value = type::value;
};

template< class C >
struct cursor_end_introversal
{
    typedef sake::cursor_introversal_tag_c<
        sake::cursor_introversal<C>::value &
        sake::end_access_introversal_tag::value
    > type;
    static unsigned int const value = type::value;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_INTROVERSAL_HPP
