/*******************************************************************************
 * sake/core/math/inverted.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERTED_HPP
#define SAKE_CORE_MATH_INVERTED_HPP

#include <boost/config.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/implicitly_defined_mem_fun.hpp>

namespace sake
{

template< class T >
struct inverted
{
    typedef T value_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_IF_MOVABLE( typename inverted, ( T ) )

    SAKE_IMPLICITLY_DEFINED_MEM_FUN(
        inverted,
        ( move_ctor ) ( copy_assign ) ( move_assign ),
        ( m_value )
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit inverted(U&& x)
        : m_value(sake::forward<U>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef typename sake::call_traits<
        typename boost_ext::add_rvalue_reference<T>::type
    >::param_type rparam_type;
    struct rv_sink_pred
    {
        template< class U >
        struct apply
        {
            static bool const value = !boost_ext::is_same_sans_qualifiers<U,T>::value;
            typedef apply type;
        };
    };
    typedef sake::rv_sink< sake::construct<T>, T, rv_sink_pred > rv_sink_type;
public:
    // lvalues
    template< class U >
    explicit inverted(U& x,
        typename boost::disable_if_c<
            boost::is_base_of<
                inverted,
                typename boost::remove_const<U>::type
            >::value
        >::type* = 0)
        : m_value(x)
    { }
    // T rvalues
    explicit inverted(rparam_type x)
        : m_value(x)
    { }
    // movable rvalues
    explicit inverted(rv_sink_type x)
        : m_value(x(sake::construct<T>()))
    { }
    // const lvalues + non-movable rvalues
    template< class U >
    explicit inverted(U const & x,
        typename boost::disable_if_c<
            boost::is_base_of< inverted, U >::value
         || boost_ext::is_same_sans_qualifiers<U,T>::value
         || boost_ext::is_convertible< U&, rv_sink_type >::value
        >::type* = 0)
        : m_value(x)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    operator U() const
    { return static_cast<U>(1) / static_cast<U>(m_value); }

private:
    T m_value;
};

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERTED_HPP
