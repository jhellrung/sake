/*******************************************************************************
 * sake/core/utility/emplacer/wrap.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * emplacer_wrap(U&& y) -> result_of::emplacer_wrap<U>::type
 * typed_emplacer_wrap<T>(U&& y) -> result_of::typed_emplacer_wrap<T,U>::type
 *
 * [typed_]emplacer_wrap is identical to the one-argument form of
 * make_[typed_]emplacer except when the argument is already an emplacer object,
 * in which case it amounts to an identity operation.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EMPLACER_WRAP_HPP
#define SAKE_CORE_UTILITY_EMPLACER_WRAP_HPP

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_void.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/utility/emplacer/emplacer.hpp>
#include <sake/core/utility/emplacer/fwd.hpp>
#include <sake/core/utility/emplacer/private/cast.hpp>
#include <sake/core/utility/emplacer/traits.hpp>

namespace sake
{

namespace emplacer_private
{

template<
    class U,
    bool = sake::is_emplacer_sans_qualifiers<U>::value
>
struct wrap_dispatch;

template<
    class T, class U,
    bool = sake::is_emplacer_sans_qualifiers<
               U, boost::mpl::quote1< boost::is_void > >::value,
    bool = sake::is_emplacer_sans_qualifiers<
               U, boost::mpl::not_< boost::is_void< boost::mpl::_1 > > >::value
>
struct typed_wrap_dispatch;

} // namespace emplacer_private

namespace result_of
{

template< class U >
struct emplacer_wrap
    : emplacer_private::wrap_dispatch<U>
{ };

template< class T, class U >
struct typed_emplacer_wrap
    : emplacer_private::typed_wrap_dispatch<T,U>
{ };

} // namespace result_of

template< class U >
inline typename sake::result_of::emplacer_wrap< SAKE_FWD2_PARAM( U ) >::type
emplacer_wrap(SAKE_FWD2_REF( U ) x)
{
    return emplacer_private::wrap_dispatch< SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x));
}

template< class T, class U >
inline typename sake::result_of::typed_emplacer_wrap< T, SAKE_FWD2_PARAM( U ) >::type
typed_emplacer_wrap(SAKE_FWD2_REF( U ) x)
{
    return emplacer_private::typed_wrap_dispatch< T, SAKE_FWD2_PARAM( U ) >::
        apply(sake::forward<U>(x));
}

namespace emplacer_private
{

template< class U >
struct wrap_dispatch< U, false >
{
    typedef sake::emplacer< void ( U ) > type;
    static type apply(typename emplacer_private::cast<U>::type x)
    { return type(emplacer_private::cast<U>::apply(x)); }
};

template< class U >
struct wrap_dispatch< U, true >
{
    typedef typename boost_ext::remove_qualifiers<U>::type type;
    static type apply(type e)
    { return e; }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, false, false >
{
    typedef sake::emplacer< T ( U ) > type;
    static type apply(typename emplacer_private::cast<U>::type x)
    { return type(emplacer_private::cast<U>::apply(x)); }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, true, false >
{
    typedef typename boost_ext::remove_qualifiers<U>::type noqual_type;
    typedef typename noqual_type::result_of::template as_typed<T>::type type;
    static type apply(noqual_type e)
    { return e.template as_typed<T>(); }
};

template< class T, class U >
struct typed_wrap_dispatch< T, U, false, true >
{
    typedef typename boost_ext::remove_qualifiers<U>::type type;
    static type apply(type e)
    { return e; }
};

} // namespace emplacer_private

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_WRAP_HPP
