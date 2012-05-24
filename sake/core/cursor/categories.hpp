/*******************************************************************************
 * sake/core/cursor/categories.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
 *
 * struct cursor_introversal_join< C0, C1 > // "or"
 * struct cursor_introversal_meet< C0, C1 > // "and"
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_CATEGORIES_HPP
#define SAKE_CORE_CURSOR_CATEGORIES_HPP

namespace sake
{

namespace cursor_introversal_tag_value
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

} // namespace cursor_introversal_tag_value

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

#define typedef_tag( tag ) \
typedef sake::cursor_introversal_tag_c< \
    sake::cursor_introversal_tag_value::tag > tag ## _introversal_tag;
typedef_tag( null )
typedef_tag( begin_detect )
typedef_tag( begin_access )
typedef_tag( end_detect )
typedef_tag( end_access )
typedef_tag( begin_detect_end_detect )
typedef_tag( begin_access_end_detect )
typedef_tag( begin_detect_end_access )
typedef_tag( begin_access_end_access )
#undef typedef_tag

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::null >
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::null;
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_detect >
    : sake::null_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_detect;
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_access >
    : sake::begin_detect_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_access;
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::end_detect >
    : sake::null_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::end_detect;
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::end_access >
    : sake::end_detect_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::end_access;
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_detect_end_detect >
    : sake::null_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_detect_end_detect;
    operator sake::begin_detect_introversal_tag() const
    { return sake::begin_detect_introversal_tag(); }
    operator sake::end_detect_introversal_tag() const
    { return sake::end_detect_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_access_end_detect >
    : sake::begin_detect_end_detect_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_access_end_detect;
    operator sake::begin_access_introversal_tag() const
    { return sake::begin_access_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_detect_end_access >
    : sake::begin_detect_end_detect_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_detect_end_access;
    operator sake::end_access_introversal_tag() const
    { return sake::end_access_introversal_tag(); }
};

template<>
struct cursor_introversal_tag_c<
    sake::cursor_introversal_tag_value::begin_access_end_access >
    : sake::begin_detect_end_detect_introversal_tag
{
    static unsigned int const value =
        sake::cursor_introversal_tag_value::begin_access_end_access;
    operator sake::begin_access_introversal_tag() const
    { return sake::begin_access_introversal_tag(); }
    operator sake::end_access_introversal_tag() const
    { return sake::end_access_introversal_tag(); }
    operator sake::begin_access_end_detect_introversal_tag() const
    { return sake::begin_access_end_detect_introversal_tag(); }
    operator sake::begin_detect_end_access_introversal_tag() const
    { return sake::begin_detect_end_access_introversal_tag(); }
};

/*******************************************************************************
 * struct cursor_introversal_join< C0, C1 > // "or"
 * struct cursor_introversal_meet< C0, C1 > // "and"
 ******************************************************************************/

template< class C0, class C1 >
struct cursor_introversal_join
{ typedef sake::cursor_introversal_tag_c< C0::value | C1::value > type; };

template< class C0, class C1 >
struct cursor_introversal_meet
{ typedef sake::cursor_introversal_tag_c< C0::value & C1::value > type; };

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_CATEGORIES_HPP
