/*******************************************************************************
 * sake/core/iterator/concepts/private/At.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_AT_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_AT_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/config.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

/*******************************************************************************
 * struct At< I, Introterminal, Tag = void >
 ******************************************************************************/

template< class I, class Introterminal, class Tag = void >
struct At
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    typedef typename traits_::template relax< Introterminal >::type relax_type;

    BOOST_CONCEPT_USAGE( At )
    {
        traits_::at_ip(i, Tag());
        assert_result< relax_type >(traits_::at(ci, Tag(), Introterminal()));
    }

private:
    I i;
    I const ci;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( At )
};

template< class I, class Introterminal >
struct At< I, Introterminal, void >
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, introterminal );
    BOOST_STATIC_ASSERT((
        boost_ext::is_convertible< introterminal, Introterminal >::value));

    typedef typename traits_::template relax< Introterminal >::type relax_type;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal< relax_type >::type,
        Introterminal
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible< I, relax_type >::value));

    BOOST_CONCEPT_USAGE( At )
    {
        traits_::at_ip(i, cj);
        assert_result<I>(traits_::at(ci, cj, introterminal()));
        assert_result< relax_type >(traits_::at(ci, cj, Introterminal()));

        typedef sake::iterator_traits< relax_type > relax_traits;
        relax_traits::at_ip(j, ci);
        assert_result< relax_type >(relax_traits::at(cj, ci, Introterminal()));
    }

private:
    I i;
    I const ci;
    relax_type j;
    relax_type const cj;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( At )
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_AT_HPP
