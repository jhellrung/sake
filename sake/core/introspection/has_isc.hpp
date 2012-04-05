/*******************************************************************************
 * sake/core/introspection/has_isc.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_ISC( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a nested integral static constants (ISC) of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_ISC( has_type_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T, class Pred = boost::mpl::always< boost::true_type > >
 * struct has_type_xxx
 * {
 *     static bool const value = [true iff T has an ISC named xxx and
 *                                {T::xxx} satisfies the Boost.MPL metafunction
 *                                Pred];
 *     typedef has_type_xxx type;
 * };
 *
 * [Here, {T::xxx} denotes a Boost.MPL integral constant with type and value
 * corresponding to T::xxx.]
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_ISC_HPP
#define SAKE_CORE_INTROSPECTION_HAS_ISC_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_ISC( trait, name ) \
template< class T, class Pred = boost::mpl::always< boost::true_type > > \
class trait \
{ \
    template< int > struct sfinae; \
    template< class T_ > static ::sake::yes_tag test(sfinae< T_::name >*); \
    template< class T_ > static ::sake::no_tag test(...); \
    static bool const has = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) ); \
    template< class T_, bool > struct apply_pred; \
    template< class T_ > \
    struct apply_pred< T_, true > \
    { \
        SAKE_EXPR_INTEGRAL_TYPEOF_TYPEDEF( typename T_::name, type ); \
        static bool const value = ::boost::mpl::apply1< \
            Pred, ::boost::integral_constant< type, T_::name > >::type::value; \
    }; \
    template< class T_ > \
    struct apply_pred< T_, false > \
    { static bool const value = false; }; \
public: \
    static bool const value = apply_pred< T, has >::value; \
    typedef trait type; \
};

#if 0 // old implementation
#include <sake/core/utility/yes_no_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_ISC( trait, name ) \
template< class T > \
class trait \
{ \
    template< int > struct sfinae; \
    template< class T_ > static ::sake::yes_tag test(sfinae< T_::name >*); \
    template< class T_ > static ::sake::no_tag test(...); \
public: \
    static bool const value = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) ); \
    typedef trait type; \
};
#endif // #if 0

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_ISC_HPP
