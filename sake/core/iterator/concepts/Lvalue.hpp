/*******************************************************************************
 * sake/core/iterator/concepts/Lvalue.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::Lvalue<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_LVALUE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_LVALUE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/is_lvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_cv.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/iterator/concepts/Readable.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct Lvalue
    : sake::iterator_concepts::Readable<I>
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );

    BOOST_STATIC_ASSERT((boost_ext::is_lvalue_reference< reference >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_same_sans_cv<
        value_type,
        typename boost_ext::remove_reference< reference >::type
    >::value));

    BOOST_CONCEPT_USAGE( Lvalue )
    {
        assert_result(*i);
    }

private:
    I const i;

    template< class T >
    typename boost::enable_if_c<
        boost::is_same< T&, reference >::value >::type
    assert_result(T&);
};

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_LVALUE_HPP
