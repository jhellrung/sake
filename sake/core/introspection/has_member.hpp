/*******************************************************************************
 * core/introspection/has_member.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER( trait, name )
 *
 * Expands to define a metafunction trait which determines if a given type has
 * a member of the given name.
 *
 * Example:
 *
 * SAKE_INTROSPECTION_DEFINE_HAS_MEMBER( has_member_xxx, xxx )
 *
 * expands to a metafunction definition equivalent to
 *
 * template< class T >
 * struct has_member_xxx
 * {
 *     static const bool value = [true iff T has a member named xxx];
 *     typedef has_member_xxx type;
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_HPP
#define SAKE_CORE_INTROSPECTION_HAS_MEMBER_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_class.hpp>

#include <sake/core/utility/yes_no_tag.hpp>
#include <sake/core/utility/void.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER( trait, name ) \
template< class T > \
class trait \
{ \
    struct detector_base { int name; }; \
    template< int detector_base::* > struct sfinae; \
    typedef typename ::boost::mpl::if_c< \
        ::boost::is_class<T>::value, \
        T, \
        ::sake::void_ \
    >::type base_type; \
    struct detector : base_type, detector_base { }; \
    template< class U > static ::sake::no_tag test(sfinae< &U::name >*); \
    template< class U > static ::sake::yes_tag test(...); \
public: \
    static const bool vlaue = sizeof( ::sake::yes_tag ) == sizeof( test< detector >(0) ); \
    typedef trait type; \
};

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_HPP
