/*******************************************************************************
 * sake/core/iterator/private/facade/operator_bracket_dispatch.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_BRACKET_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_BRACKET_DISPATCH_HPP

#include <boost/config.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/core/introspection/has_operator_assign.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>
#include <sake/core/utility/noncopy_assignable.hpp>

namespace sake
{

namespace iterator_facade_private
{

/*******************************************************************************
 * operator[] must return a proxy in case iterator destruction invalidates
 * referents.
 * To see why, consider the following implementation of operator[]:
 *     reference operator[](difference_param_type n) const
 *     { return *(*this + n); }
 * The problem here is that operator[] would return a reference created from
 * a temporary iterator.
 ******************************************************************************/

template<
    class Value, class Reference, class I,
    bool = boost::has_trivial_copy< Value >::value
        || sake::is_by_value_optimal< Value >::value
>
struct operator_bracket_dispatch
{
    class proxy
    {
        I const m_i;
        proxy(I const & i) : m_i(i) { }
        friend struct operator_bracket_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )
        operator Reference() const
        { return *m_i; }

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
            Value,
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
        // Value rvalues
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

template< class Value, class Reference, class I >
struct operator_bracket_dispatch< Value const, Reference, I, false >
{
    class proxy
    {
        I const m_i;
        proxy(I const & i) : m_i(i) { }
        friend struct operator_bracket_dispatch;
    public:
        SAKE_NONCOPY_ASSIGNABLE( proxy )
        operator Reference() const
        { return *m_i; }
    };

    typedef proxy result_type;
    static result_type apply(I const & i)
    { return proxy(i); }
};

template< class Value, class Reference, class I >
struct operator_bracket_dispatch< Value const, Reference, I, true >
{
    typedef Value result_type;
    static result_type apply(I const & i)
    { return *i; }
};

} // namespace iterator_facade_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_BRACKET_DISPATCH_HPP
