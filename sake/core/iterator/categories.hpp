/*******************************************************************************
 * sake/core/iterator/categories.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct introversal_tag_c< Value >
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
 * struct introversal_join< C0, C1 > // "or"
 * struct introversal_meet< C0, C1 > // "and"
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CATEGORIES_HPP
#define SAKE_CORE_ITERATOR_CATEGORIES_HPP

#include <boost/iterator/iterator_categories.hpp>

namespace sake
{

namespace introversal_tag_value
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

} // namespace introversal_tag_value

/*******************************************************************************
 * struct introversal_tag_c< Value >
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
struct introversal_tag_c;

#define typedef_tag( tag ) \
typedef sake::introversal_tag_c< sake::introversal_tag_value::tag > tag ## _introversal_tag;
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

#define declare_tag( tag ) \
template<> struct introversal_tag_c< sake::introversal_tag_value::tag >
#define define_value( tag ) \
    static unsigned int const value = sake::introversal_tag_value::tag
#define define_operator( tag ) \
    operator sake::tag ## _introversal_tag() const \
    { return sake::tag ## _introversal_tag(); }

declare_tag( null )
{ define_value( null ); };

declare_tag( begin_detect )
    : sake::null_introversal_tag
{ define_value( begin_detect ); };

declare_tag( begin_access )
    : sake::begin_detect_introversal_tag
{ define_value( begin_access ); };

declare_tag( end_detect )
    : sake::null_introversal_tag
{ define_value( end_detect ); };

declare_tag( end_access )
    : sake::end_detect_introversal_tag
{ define_value( end_access ); };


declare_tag( begin_detect_end_detect )
    : sake::null_introversal_tag
{
    define_value( begin_detect_end_detect );
    define_operator( begin_detect )
    define_operator( end_detect )
};

declare_tag( begin_access_end_detect )
    : sake::begin_detect_end_detect_introversal_tag
{
    define_value( begin_access_end_detect );
    define_operator( begin_access )
};

declare_tag( begin_detect_end_access )
    : sake::begin_detect_end_detect_introversal_tag
{
    define_value( begin_detect_end_access );
    define_operator( end_access )
};

declare_tag( begin_access_end_access )
    : sake::begin_detect_end_detect_introversal_tag
{
    define_value( begin_access_end_access );
    define_operator( begin_access )
    define_operator( end_access )
    define_operator( begin_access_end_detect )
    define_operator( begin_detect_end_access )
};

#undef declare_tag
#undef define_value
#undef define_operator

/*******************************************************************************
 * struct introversal_join< C0, C1 > // "or"
 * struct introversal_meet< C0, C1 > // "and"
 ******************************************************************************/

template< class C0, class C1 >
struct introversal_join
{
    static unsigned int const value = C0::value | C1::value;
    typedef sake::introversal_tag_c< value > type;
};

template< class C0, class C1 >
struct introversal_meet
{
    static unsigned int const value = C0::value & C1::value;
    typedef sake::introversal_tag_c< value > type;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CATEGORIES_HPP
