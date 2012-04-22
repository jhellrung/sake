/*******************************************************************************
 * sake/core/iterator/private/facade/operator_post_increment_dispatch.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * The logic in here roughly coincides with that used in boost::iterator_facade.
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP

#include <boost/config.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>

namespace sake
{

namespace iterator_facade_private
{

/*******************************************************************************
 * operator++(int) must occasionally return a proxy, in case iterators within
 * the same sequence share the same referent.  Consider situations in which
 * incrementing changes the value of the (common) referent.
 ******************************************************************************/

template<
    class Value, class Reference, class Traversal, class I,
    bool = boost_ext::mpl::and3<
        boost::is_object< Value >,
        // Multipass iterators don't have the issue described above.
        boost::mpl::not_< boost_ext::is_convertible<
            Traversal,
            boost::forward_traversal_tag
        > >,
        // Only readable iterators need a proxy.
        boost_ext::is_convertible<
            Reference,
            typename boost_ext::add_reference_add_const< Value >::type
        >
    >::value,
    // Dispatch on whether the reference type is a proxy.
    bool = boost_ext::is_convertible<
        typename boost_ext::remove_qualifiers< Reference >::type *,
        typename boost::remove_const< Value >::type *
    >::value
>
struct operator_post_increment_dispatch
{
    typedef I result_type;
    static result_type apply(I const & i)
    { return i; }
};

template< class Value, class Reference, class Traversal, class I >
struct operator_post_increment_dispatch< Value, Reference, Traversal, I, true, false >
{
    typedef typename boost::remove_const< Value >::type value_type;

    class proxy
    {
        I const m_i;
        mutable value_type m_x;
        proxy(I const & i) : m_i(i), m_x(*i) { }
        friend struct operator_post_increment_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )

        operator I() const
        { return m_i; }
        // We return a reference-to-non-const to allow mutating operations, even
        // though the results of such mutations will (probably) be discarded.
        operator value_type&() const
        { return m_x; }

        // Dereferencing must return a proxy, with assignations forwarded to the
        // *m_i.
        proxy const & operator*() const
        { return *this; }

    private:
        template< class T >
        struct enable_cond_operator_assign
            : sake::has_operator_assign< Reference, void ( T ) >
        { };
        template< class T >
        struct enable_operator_assign
            : boost::enable_if_c<
                  enable_cond_operator_assign<T>::value,
                  proxy const &
              >
        { };
    public:

#ifndef BOOST_NO_RVALUE_REFERENCES

        template< class T >
        typename enable_operator_assign<T>::type
        operator=(T&& x) const
        { *m_i = sake::forward<T>(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    private:
        typedef sake::rv_sink_traits1<
            value_type,
            boost::mpl::quote1< enable_cond_operator_assign >
        > operator_assign_rv_sink_traits;
        typedef typename operator_assign_rv_sink_traits::template
            default_< sake::rv_sink_visitors::operator_assign< proxy const > >
            operator_assign_rv_sink_default_type;
    public:
        // lvalues + movable explicit rvalues
        template< class T >
        typename operator_assign_rv_sink_traits::template
            enable_ref< T, proxy const & >::type
        operator=(T& x) const
        { *m_i = x; return *this; }
        // value_type rvalues
        proxy const &
        operator=(typename operator_assign_rv_sink_traits::primary_type x) const
        { *m_i = x.move(); return *this; }
        // movable implicit rvalues
        proxy const &
        operator=(operator_assign_rv_sink_default_type x) const
        { return x(sake::rv_sink_visitors::make_operator_assign(*this)); }
        // const lvalues + non-movable rvalues
        template< class T >
        typename operator_assign_rv_sink_traits::template
            enable_cref< T, proxy const & >::type
        operator=(T const & x) const
        { *m_i = x; return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    };

    typedef proxy result_type;
    static result_type apply(I const & i)
    { return proxy(i); }
};

template< class Value, class Reference, class Traversal, class I >
struct operator_post_increment_dispatch< Value, Reference, Traversal, I, true, true >
{
    typedef typename boost::remove_const< Value >::type value_type;

    class proxy
    {
        I const m_i;
        mutable value_type m_x;
        proxy(I const & i) : m_i(i), m_x(*i) { }
        friend struct operator_post_increment_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )
        operator I() const
        { return m_i; }
        value_type& operator*() const
        { return m_x; }
    };

    typedef proxy result_type;
    static result_type apply(I const & i)
    { return proxy(i); }
};

} // namespace iterator_facade_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_POST_INCREMENT_DISPATCH_HPP
