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
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/is_by_value_optimal.hpp>
#include <sake/core/utility/non_copy_assignable.hpp>

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
        SAKE_NON_COPY_ASSIGNABLE( proxy )
        operator Reference() const
        { return *m_i; }

#ifndef BOOST_NO_RVALUE_REFERENCES

        template< class T >
        proxy const &
        operator=(T&& x) const
        { *m_i = sake::forward<T>(x); return *this; }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    private:
        typedef typename sake::rv_sink_traits::rv_param< Value >::type rv_param_type;
        typedef sake::rv_sink<
            sake::rv_sink_visitors::operator_assign< proxy const >, // Visitor
            proxy const &, // Result
            boost::mpl::not_< boost::is_same< Value, boost::mpl::_1 > > // Pred
        > operator_assign_rv_sink_type;
    public:
        // lvalues + movable explicit rvalues
        template< class T >
        proxy const &
        operator=(T& x) const
        { *m_i = x; return *this; }
        // Value rvalues
        proxy const &
        operator=(rv_param_type x) const
        { *m_i = x; return *this; }
        // movable implicit rvalues
        proxy const &
        operator=(operator_assign_rv_sink_type x) const
        { return x(sake::rv_sink_visitors::make_operator_assign(*this)); }
        // const lvalues + non-movable rvalues
        template< class T >
        typename boost::disable_if_c<
            boost_ext::mpl::or2<
                boost_ext::is_same_sans_qualifiers< T, rv_param_type >,
                sake::is_movable<T>
            >::value,
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

template< class Value, class Reference, class I >
struct operator_bracket_dispatch< Value const, Reference, I, false >
{
    class proxy
    {
        I const m_i;
        proxy(I const & i) : m_i(i) { }
        friend struct operator_bracket_dispatch;
    public:
        SAKE_NON_COPY_ASSIGNABLE( proxy )
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
