/*******************************************************************************
 * sake/core/iterator/private/facade/end_introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/common_base.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class Params >
struct end_introversal_base_index
{
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, cursor_introversal );
    static int const value =
        boost_ext::is_convertible< cursor_introversal, sake::end_access_introversal_tag >::value
      + boost_ext::is_convertible< cursor_introversal, sake::end_detect_introversal_tag >::value;
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 0 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( common_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< end_introversal_base, T >::value
        >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 1 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
public:
    using common_base_::derived;

    bool at_end() const
    { return sake::iterator_core_access::at_end(derived()); }
    inline friend bool
    at_end(Derived const & this_)
    { return this_.at_end(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( common_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< end_introversal_base, T >::value
        >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 2 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
public:
    using common_base_::derived;

    bool at_end() const
    { return sake::iterator_core_access::at_end(derived()); }
    inline friend bool
    at_end(Derived const & this_)
    { return this_.at_end(); }

    Derived end() const
    { return sake::iterator_core_access::end(derived()); }
    inline friend Derived
    end(Derived const & this_)
    { return this_.end(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( common_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< end_introversal_base, T >::value
        >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator_core_access;

    bool at_end_impl() const
    { return derived() == derived().end(); }
};

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP
