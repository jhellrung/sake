/*******************************************************************************
 * sake/core/math/inverse.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct inverse<T>
 *
 * Defines a simple wrapper class representing the (multiplicative) inverse of
 * a number.
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INVERSE_HPP
#define SAKE_CORE_MATH_INVERSE_HPP

#include <boost/config.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/functional/construct.hpp>
#include <sake/core/math/inv_fwd.hpp>
#include <sake/core/math/inverse_fwd.hpp>
#include <sake/core/math/one.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/emplacer/construct.hpp>
#include <sake/core/utility/emplacer/traits.hpp>
#include <sake/core/utility/memberwise/swap.hpp>

namespace sake
{

namespace inverse_adl
{

/*******************************************************************************
 * struct inverse<T>
 ******************************************************************************/

template< class T >
struct inverse
{
    BOOST_STATIC_ASSERT((!boost_ext::is_reference<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    typedef T value_type;

    SAKE_OPTIMAL_MOVABLE_COPYABLE_MEMBERWISE(
        typename inverse,
        (( T )( m_value ))
    )

    SAKE_MEMBERWISE_SWAP(
        typename inverse,
        (( T )( m_value ))
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    explicit inverse(U&& x,
        typename boost::enable_if_c<
            boost_ext::is_convertible<U,T>::value >::type* = 0)
        : m_value(sake::forward<U>(x))
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef typename sake::rv_sink_traits::rv_param<T>::type rv_param_type;
    typedef sake::rv_sink<
        sake::functional::construct<T>, // Visitor
        T, // Result
        boost::mpl::and2<
            boost::mpl::not_< boost::is_same< T, boost::mpl::_1 > >,
            boost_ext::is_convertible< boost::mpl::_1, T >
        > // Pred
    > ctor_rv_sink_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    explicit inverse(U& x,
        typename boost::enable_if_c<
            boost_ext::is_convertible< U&, T >::value >::type* = 0)
        : m_value(x)
    { }
    // T rvalues
    explicit inverse(rv_param_type x)
        : m_value(x)
    { }
    // movable implicit rvalues
    explicit inverse(ctor_rv_sink_type x)
        : m_value(x())
    { }
    // const lvalues + non-movable rvalues
    template< class U >
    explicit inverse(U const & x,
        typename boost::disable_if_c< boost_ext::mpl::or3<
            boost_ext::is_same_sans_qualifiers< U, rv_param_type >,
            sake::is_movable<U>,
            boost::mpl::not_< boost_ext::is_convertible< U const &, T > >
        >::value >::type* = 0)
        : m_value(x)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class Emplacer >
    explicit inverse(Emplacer e,
        typename sake::enable_if_is_emplacer< Emplacer >::type* = 0)
        : m_value(emplacer_construct<T>(e))
    { }

    T const & value() const
    { return m_value; }

    template< class U >
    operator U() const
    { return sake::one.as<U>() / static_cast<U>(m_value); }

    inline friend
    T inv(inverse const & this_)
    { return this_.m_value; }

    inline friend
    T inv(this_rvalue_param_type this_)
    { return sake::move(this_.m_value); }

private:
    T m_value;
};

} // namespace inverse_adl

/*******************************************************************************
 * struct result_of::extension::inv< inverse<T> >
 ******************************************************************************/

namespace result_of {
namespace extension {

template< class T >
struct inv< sake::inverse<T>, void >
{ typedef T type; };

} // namespace extension
} // namespace result_of

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INVERSE_HPP
