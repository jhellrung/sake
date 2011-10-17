/*******************************************************************************
 * sake/core/iterator/private/facade/postincrement_dispatch.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * The logic in here roughly coincides with that used in boost::iterator_facade.
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_POSTINCREMENT_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_POSTINCREMENT_DISPATCH_HPP

#include <boost/config.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/non_copy_assignable.hpp>

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
        typename boost::remove_cv< Value >::type *
    >::value
>
struct postincrement_dispatch
{
    typedef I result_type;
    static result_type apply(I const & i)
    { return i; }
};

template< class Value, class Reference, class Traversal, class I >
struct postincrement_dispatch< Value, Reference, Traversal, I, true, false >
{
    typedef typename boost::remove_const< Value >::type value_type;

    class proxy
    {
        I const m_i;
        mutable value_type m_x;
        proxy(I const & i) : m_i(i), m_x(*i) { }
        friend struct postincrement_dispatch;
    public:
        SAKE_NON_COPY_ASSIGNABLE( proxy )

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

#ifndef BOOST_NO_RVALUE_REFERENCES

        template< class T >
        proxy const &
        operator=(T&& x) const
        { *m_i = sake::forward<T>(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    private:
        typedef typename sake::call_traits<
            typename boost_ext::add_rvalue_reference< value_type >::type
        >::type rparam_type;
        struct binder
        {
            explicit binder(proxy const & this_) : m_this(this_) { }
            typedef proxy const & result_type;
            template< class T >
            result_type operator()(T& x) const
            { return m_this = x; }
        private:
            proxy const & m_this;
        };
        struct rv_sink_pred
        {
            template< class T >
            struct apply
            {
                static bool const value = !boost::is_same< T, value_type >::value;
                typedef apply type;
            };
        };
        typedef sake::rv_sink< binder, proxy const &, rv_sink_pred > rv_sink_type;
    public:
        // lvalues
        template< class T >
        proxy const &
        operator=(T& x) const
        { *m_i = x; return *this; }
        // value_type rvalues
        template< class T >
        proxy const &
        operator=(rparam_type x) const
        { *m_i = x; return *this; }
        // movable rvalues
        proxy const &
        operator=(any_rv_type x) const
        { return x(binder(*this)); }
        // const lvalues + non-movable rvalues
        template< class T >
        typename boost::disable_if_c<
            boost::is_same< T, value_type >::value
         || boost_ext::is_convertible< T&, rv_sink_type >::value,
            proxy const &
        >::type
        operator=(T const & x) const
        { *m_i = x; return *this; }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    };

    typedef proxy result_type;
    static result_type apply(I const & i)
    { return proxy(i); }
};

template< class Value, class Reference, class Traversal, class I >
struct postincrement_dispatch< Value, Reference, Traversal, I, true, true >
{
    typedef typename boost::remove_const< Value >::type value_type;

    class proxy
    {
        I const m_i;
        mutable value_type m_x;
        proxy(I const & i) : m_i(i), m_x(*i) { }
        friend struct postincrement_dispatch;
    public:
        SAKE_NON_COPY_ASSIGNABLE( proxy )
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

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_POSTINCREMENT_DISPATCH_HPP
