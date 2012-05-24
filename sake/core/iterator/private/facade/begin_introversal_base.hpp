/*******************************************************************************
 * sake/core/iterator/private/facade/begin_introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROVERSAL_BASE_HPP

#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/cursor/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/end_introversal_base.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

template< class Params >
struct begin_introversal_base_index
{
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, cursor_introversal );
    static int const value =
        boost_ext::is_convertible< cursor_introversal, sake::begin_access_introversal_tag >::value
      + boost_ext::is_convertible< cursor_introversal, sake::begin_detect_introversal_tag >::value;
};

template< class Derived, class Params >
class begin_introversal_base< Derived, Params, 0 >
    : public private_::end_introversal_base< Derived, Params >
{
    typedef private_::end_introversal_base<
        Derived, Params
    > end_introversal_base_;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introversal_base,
        (( end_introversal_base_ ))
    )

    template< class T >
    explicit begin_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< begin_introversal_base, T >::value
        >::type* = 0)
        : end_introversal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class begin_introversal_base< Derived, Params, 1 >
    : public private_::end_introversal_base< Derived, Params >
{
    typedef private_::end_introversal_base<
        Derived, Params
    > end_introversal_base_;
protected:
    using end_introversal_base_::derived;
public:

    bool at_begin() const
    { return sake::iterator_core_access::at_begin(derived()); }
    inline friend bool
    at_begin(Derived const & this_)
    { return this_.at_begin(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introversal_base,
        (( end_introversal_base_ ))
    )

    template< class T >
    explicit begin_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< begin_introversal_base, T >::value
        >::type* = 0)
        : end_introversal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class begin_introversal_base< Derived, Params, 2 >
    : public private_::end_introversal_base< Derived, Params >
{
    typedef private_::end_introversal_base<
        Derived, Params
    > end_introversal_base_;
protected:
    using end_introversal_base_::derived;
public:

    bool at_begin() const
    { return sake::iterator_core_access::at_begin(derived()); }
    inline friend bool
    at_begin(Derived const & this_)
    { return this_.at_begin(); }

    Derived begin() const
    { return sake::iterator_core_access::begin(derived()); }
    inline friend Derived
    begin(Derived const & this_)
    { return this_.begin(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introversal_base,
        (( end_introversal_base_ ))
    )

    template< class T >
    explicit begin_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c<
            boost_ext::is_base_of_sans_qualifiers< begin_introversal_base, T >::value
        >::type* = 0)
        : end_introversal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator_core_access;

    bool at_begin_impl() const
    { return derived() == derived().begin(); }
};

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROVERSAL_BASE_HPP
