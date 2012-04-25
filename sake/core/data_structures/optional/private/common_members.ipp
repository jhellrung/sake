/*******************************************************************************
 * sake/core/data_structures/optional/private/common_members.ipp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>

#include <sake/core/data_structures/optional/traits.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/cast/implicit.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>
#include <sake/core/utility/emplacer/traits.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>

#endif // #ifdef SAKE_OPTIONAL_INCLUDE_HEADERS

#ifdef SAKE_OPTIONAL_DEFINE_MEMBERS

private:
    template< class U >
    struct value_enable
        : sake::is_convertible_wnrbt< U, value_type >
    { };

    template< class U, class Result = void >
    struct value_enabler
        : boost::enable_if_c< value_enable<U>::value, Result >
    { };

    template< class U >
    struct optional_enable
        : boost_ext::mpl::or2<
              boost_ext::is_same_sans_qualifiers< U, optional >,
              boost_ext::mpl::and2<
                  boost::mpl::not_< sake::is_convertible_wnrbt< U, value_type > >,
                  sake::is_optional_sans_qualifiers<
                      U,
                      sake::is_convertible_wnrbt<
                          boost_ext::propagate_qualifiers< U, boost::mpl::_1 >,
                          value_type
                      >
                  >
              >
          >
    { };

    template< class U, class Result = void >
    struct optional_enabler
        : boost::enable_if_c< optional_enable<U>::value, Result >
    { };

    template< class U >
    struct emplacer_enable
        : sake::is_emplacer_sans_qualifiers_with_value< U, value_type >
    { };

    template< class U, class Result = void >
    struct emplacer_enabler
        : boost::enable_if_c< emplacer_enable<U>::value, Result >
    { };
public:

#ifdef SAKE_OPTIONAL_REFERENCE

private:
    T* get_ptr_dispatch(T& x)
    { return sake::address_of(x); }

    template< class U >
    typename optional_enabler< U&, T* >::type
    get_ptr_dispatch(U& x)
    { return get_ptr_dispatch(x.get_ptr()); }

    template< class U >
    typename boost::enable_if_c<
        boost_ext::is_convertible< U*, T* >::value, T* >::type
    get_ptr_dispatch(U* const p)
    { return p; }

    template< class U >
    typename boost::disable_if_c<
        boost_ext::is_convertible< U*, T* >::value, T* >::type
    get_ptr_dispatch(U* const p)
    { return p ? sake::address_of(sake::implicit_cast< T& >(*p)) : 0; }

    template< class Signature >
    T* get_ptr_dispatch(sake::emplacer< Signature > e)
    { return sake::address_of(sake::emplacer_construct< T& >(e)); }
public:

#endif // #ifndef SAKE_OPTIONAL_REFERENCE

#endif // #ifdef SAKE_OPTIONAL_DEFINE_MEMBERS
