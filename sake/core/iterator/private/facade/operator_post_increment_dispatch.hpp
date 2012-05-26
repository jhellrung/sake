/*******************************************************************************
 * sake/core/iterator/private/facade/operator_post_increment_dispatch.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * The logic in here roughly coincides with that used in boost::iterator_facade.
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>

namespace sake
{

namespace iterator_facade_adl
{

namespace private_
{

/*******************************************************************************
 * operator++(int) must occasionally return a proxy, in case iterators within
 * the same sequence share the same referent.  Consider situations in which
 * incrementing changes the value of the (common) referent.
 ******************************************************************************/

template< class Value, class Reference, class Traversal >
struct operator_post_increment_dispatch_index
{
    static int const value = boost_ext::mpl::
    if_<
        boost_ext::mpl::or3<
            boost::mpl::not_< boost::is_object< Value > >,
            // Multipass iterators don't have the issue described above.
            boost_ext::is_convertible<
                Traversal, boost::forward_traversal_tag >,
            // Only readable iterators need a proxy.
            boost::mpl::not_< boost_ext::is_convertible<
                Reference,
                typename boost_ext::add_reference_add_const< Value >::type
            > >
        >,
        sake::int_tag<2>
    >::type::template
    else_if<
        // Dispatch on whether the reference type is a proxy.
        boost_ext::is_convertible<
            typename boost_ext::remove_qualifiers< Reference >::type *,
            typename boost::remove_const< Value >::type *
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type::value;
};

template<
    class This, class Value, class Reference, class Traversal,
    int = operator_post_increment_dispatch_index<
        Value, Reference, Traversal >::value
>
struct operator_post_increment_dispatch;

template< class This, class Value, class Reference, class Traversal >
struct operator_post_increment_dispatch< This, Value, Reference, Traversal, 2 >
{
    typedef This type;
    static type apply(This const & this_)
    { return this_; }
};

template< class This, class Value, class Reference, class Traversal >
struct operator_post_increment_dispatch< This, Value, Reference, Traversal, 1 >
{
    class proxy
    {
        typedef typename boost::remove_const< Value >::type value_type;
        This const m_this;
        mutable value_type m_x;
        explicit proxy(This const & this_) : m_this(this_), m_x(*this_) { }
        friend struct operator_post_increment_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )
        operator This() const { return m_this; }
        value_type& operator*() const { return m_x; }
    };

    typedef proxy type;
    static type apply(This const & this_)
    { return proxy(this_); }
};

template< class This, class Value, class Reference, class Traversal >
struct operator_post_increment_dispatch< This, Value, Reference, Traversal, 0 >
{
    typedef typename boost::remove_const< Value >::type value_type;

    class proxy
    {
        This const m_this;
        mutable value_type m_x;
        explicit proxy(This const & this_) : m_this(this_), m_x(*this_) { }
        friend struct operator_post_increment_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )

        operator This() const { return m_this; }
        // We return a reference-to-non-const to allow mutating operations, even
        // though the results of such mutations will (probably) be discarded.
        operator value_type& () const { return m_x; }

        // Dereferencing must return a proxy, with assignations forwarded to
        // *m_this.
        proxy const & operator*() const { return *this; }

    private:
        template< class T >
        struct operator_assign_enable
            : sake::has_operator_assign< Reference, void ( T ) >
        { };
        template< class T >
        struct operator_assign_enabler
            : boost::enable_if_c<
                  operator_assign_enable<T>::value, proxy const & >
        { };
    public:

#ifndef BOOST_NO_RVALUE_REFERENCES

        template< class T >
        typename operator_assign_enabler<T>::type
        operator=(T&& x) const
        { *m_this = sake::forward<T>(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    private:
        typedef sake::rv_sink_traits1<
            value_type, boost::mpl::quote1< operator_assign_enable >
        > operator_assign_rv_sink_traits;
        typedef typename operator_assign_rv_sink_traits::template
            default_< sake::rv_sink_visitors::operator_assign< proxy const > >
            operator_assign_rv_sink_default_type;
    public:
        // lvalues + movable explicit rvalues
        template< class T >
        typename operator_assign_rv_sink_traits::template
            ref_enabler< T, proxy const & >::type
        operator=(T& x) const
        { *m_this = x; return *this; }
        // value_type rvalues
        proxy const &
        operator=(typename operator_assign_rv_sink_traits::primary_type x) const
        { *m_this = sake::move(x.value); return *this; }
        // movable implicit rvalues
        proxy const &
        operator=(operator_assign_rv_sink_default_type x) const
        { return x(sake::rv_sink_visitors::make_operator_assign(*this)); }
        // const lvalues + non-movable rvalues
        template< class T >
        typename operator_assign_rv_sink_traits::template
            cref_enabler< T, proxy const & >::type
        operator=(T const & x) const
        { *m_this = x; return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    };

    typedef proxy type;
    static type apply(This const & this_)
    { return proxy(this_); }
};

} // namespace private_

} // namespace iterator_facade_adl

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP
