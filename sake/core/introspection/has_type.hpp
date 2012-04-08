/*******************************************************************************
 * sake/core/introspection/has_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_TYPE( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a nested typedef or struct of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_TYPE( has_type_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T, class Pred = boost::mpl::always< boost::true_type > >
 * struct has_type_xxx
 * {
 *     static bool const value = [true iff T has a nested type named xxx and
 *                                T::xxx satisfies the Boost.MPL metafunction
 *                                Pred];
 *     typedef has_type_xxx type;
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP
#define SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/utility/sizeof_t.hpp>
#include <sake/core/utility/true_false_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_TYPE( trait, name ) \
template< class T, class Pred = boost::mpl::always< boost::true_type > > \
class trait \
{ \
    template< class U > struct sfinae \
        : ::sake::sizeof_t< 1 + ::boost::mpl::apply1< Pred, U >::type::value > { }; \
    template< class T_ > static sfinae< typename T_::name > test(int); \
    template< class T_ > static ::sake::false_tag test(...); \
public: \
    static bool const value = SAKE_SIZEOF_TRUE_TAG == sizeof( test<T>(0) ); \
    typedef trait type; \
};

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_TYPE_HPP
