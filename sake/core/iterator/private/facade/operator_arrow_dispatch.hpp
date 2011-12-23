/*******************************************************************************
 * sake/core/iterator/private/facade/operator_arrow_dispatch.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_ARROW_DISPATCH_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_ARROW_DISPATCH_HPP

#include <sake/core/utility/address_of.hpp>

namespace sake
{

namespace iterator_facade_private
{

/*******************************************************************************
 * If Reference is a proxy reference, then operator-> must return a proxy
 * wrapper containing the proxy reference.
 * To see why, consider the following implementation of operator->:
 *     pointer operator->() const
 *     { return sake::address_of(static_cast< Value& >(operator*())); }
 * The problem here is that operator-> returns a pointer to a temporary object
 * if operator*() returns an rvalue (i.e., a proxy reference).
 ******************************************************************************/

template< class Reference >
struct operator_arrow_dispatch
{
    class proxy
    {
        Reference const m_ref;
        explicit proxy(Reference const & ref) : m_ref(ref) { }
        friend struct operator_arrow_dispatch;
    public:
        Reference const * operator->() const
        { return sake::address_of(m_ref); }
    };
    typedef proxy result_type;
    result_type apply(Reference const & x)
    { return result_type(x); }
};

template< class T >
struct operator_arrow_dispatch< T& >
{
    typedef typename sake::result_of::address_of< T& >::type result_type;
    result_type apply(T& x)
    { return sake::address_of(x); }
};

} // namespace iterator_facade_private

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_OPERATOR_ARROW_DISPATCH_HPP
