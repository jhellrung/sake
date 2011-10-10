/*******************************************************************************
 * sake/core/keyword/tagged_lazy.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_KEYWORD_TAGGED_LAZY_HPP
#define SAKE_CORE_KEYWORD_TAGGED_LAZY_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/utility/result_of.hpp>

#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace keyword
{

template< class Tag, class T >
struct tagged_lazy
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    typedef Tag tag;
    typedef typename boost::result_of< T const ( ) >::type value_type;
    explicit tagged_lazy(typename sake::call_traits<T>::param_type value_)
        : m_value(value_)
    { }
    value_type value() const
    { return m_value(); }
private:
    T const m_value;
};

} // namespace keyword

} // namespace sake

#endif // #ifndef SAKE_CORE_KEYWORD_TAGGED_LAZY_HPP
