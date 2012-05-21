/*******************************************************************************
 * sake/core/cast/polymorphic_down.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * polymorphic_downcast< D* >(B* p) -> D*
 * struct functional::polymorphic_downcast< D* >
 * struct functional::polymorphic_downcast<>
 ******************************************************************************/

#ifndef SAKE_CORE_CAST_POLYMORPHIC_DOWN_HPP
#define SAKE_CORE_CAST_POLYMORPHIC_DOWN_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/utility/assert.hpp>
#include <sake/core/utility/debug.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace result_of
{

template< class B, class D >
struct polymorphic_downcast;

} // namespace result_of

namespace polymorphic_downcast_private
{

template< class D, class B > struct enable;

} // namespace polymorphic_downcast_private

template< class D, class B >
inline typename polymorphic_downcast_private::enable< D, B* >::type
polymorphic_downcast(B* const p)
{ return sake::polymorphic_downcast(p, sake::type_tag<D>()); }

template< class B, class D >
inline typename polymorphic_downcast_private::enable< D*, B* >::type
polymorphic_downcast(B* const p, sake::type_tag< D* >)
{
    BOOST_STATIC_ASSERT((boost::is_base_of<B,D>::value));
#ifdef SAKE_DEBUG
    D* const q = dynamic_cast< D* >(p);
    SAKE_ASSERT_RELATION( p, ==, q );
    return q;
#else // #ifdef SAKE_DEBUG
    return static_cast< D* >(p);
#endif // #ifdef SAKE_DEBUG
}

namespace functional
{

template< class D = void >
struct polymorphic_downcast;

template< class D >
struct polymorphic_downcast< D* >
{
    typedef D* result_type;
    template< class B >
    typename polymorphic_downcast_private::enable< D*, B* >::type
    operator()(B* const p) const
    { return sake::polymorphic_downcast< D* >(p); }
};

template<>
struct polymorphic_downcast< void >
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::result_of::polymorphic_downcast, 2 )

    template< class B, class D >
    typename polymorphic_downcast_private::enable< D*, B* >::type
    operator()(B* const p, sake::type_tag< D* >) const
    { return sake::polymorphic_downcast< D* >(p); }
};

} // namespace functional

namespace result_of
{

namespace polymorphic_downcast_private
{

template< class B, class D >
struct impl;

template< class B, class D >
struct impl< B*, D* >
{
    BOOST_STATIC_ASSERT((boost::is_polymorphic<B>::value));
    BOOST_STATIC_ASSERT((boost::is_base_of<B,D>::value));
    typedef D* type;
};

} // namespace polymorphic_downcast_private

template< class B, class D >
struct polymorphic_downcast
    : polymorphic_downcast_private::impl<
          typename boost_ext::remove_qualifiers<B>::type,
          typename sake::remove_qualified_type_tag<D>::type
      >
{ };

} // namespace result_of

namespace polymorphic_downcast_private
{

template< class D, class B >
struct enable
{ };

template< class D, class B >
struct enable< D*, B* >
    : boost::enable_if_c<
          boost_ext::mpl::and2<
              boost::is_polymorphic<B>,
              boost::is_base_of<B,D>
          >::value,
          D*
      >
{ };

} // namespace polymorphic_downcast_private

} // namespace sake

#endif // #ifndef SAKE_CORE_CAST_POLYMORPHIC_DOWN_HPP
