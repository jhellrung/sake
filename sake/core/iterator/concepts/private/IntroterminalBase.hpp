/*******************************************************************************
 * sake/core/iterator/concepts/private/IntroterminalBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/EqualityComparable.hpp>
#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

namespace private_
{

template<
    class I,
    class Introterminal = typename sake::iterator_introterminal<I>::type
>
struct IntroterminalBase;

/*******************************************************************************
 * struct IntroterminalAt< I, Introterminal, Tag = void >
 ******************************************************************************/

template< class I, class Introterminal, class Tag = void >
struct IntroterminalAt
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    typedef typename traits_::template relax< Introterminal >::type relax_type;

    BOOST_CONCEPT_USAGE( IntroterminalAt )
    {
        traits_::at_ip(i, Tag());
        assert_result< relax_type >(traits_::at(ci, Tag(), Introterminal()));
    }

private:
    I i;
    I const ci;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroterminalAt )
};

template< class I, class Introterminal >
struct IntroterminalAt< I, Introterminal, void >
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

    BOOST_CONCEPT_USAGE( IntroterminalAt )
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

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroterminalAt )
};

/*******************************************************************************
 * struct IntroterminalDifference< I, Tag >
 ******************************************************************************/

template<
    class I, class Tag,
    bool = boost_ext::is_convertible<
               typename sake::iterator_traversal<I>::type,
               boost::random_access_traversal_tag
           >::value
>
struct IntroterminalDifference;

template< class I, class Tag >
struct IntroterminalDifference< I, Tag, false >
{ };

template< class I, class Tag >
struct IntroterminalDifference< I, Tag, true >
{
    typedef typename sake::iterator_difference<I>::type difference_type;

    BOOST_CONCEPT_USAGE( IntroterminalDifference )
    {
        assert_result(i - Tag());
        assert_result(Tag() - i);
    }

private:
    I const i;

    void assert_result(difference_type);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroterminalDifference )
};

/*******************************************************************************
 * struct IntroterminalRelax< I, Introterminal >
 ******************************************************************************/

template<
    class I, class Introterminal,
    class Relax = typename sake::iterator_relax< I, Introterminal >::type
>
struct IntroterminalRelax
#ifdef SAKE_ITERATOR_CONCEPT_COMPLETE
    : sake::iterator::concepts::private_::IntroterminalBase< Relax, Introterminal >
#else // #ifdef SAKE_ITERATOR_CONCEPT_COMPLETE
    : sake::iterator::concepts::private_::Base< Relax >
#endif // #ifdef SAKE_ITERATOR_CONCEPT_COMPLETE
{
private:
    typedef sake::iterator_traits<I> traits_;
    typedef sake::iterator_traits< Relax > relax_traits;
public:
    SAKE_USING_TYPEDEF( typename traits_, value_type );
    SAKE_USING_TYPEDEF( typename traits_, reference );
    SAKE_USING_TYPEDEF( typename traits_, difference_type );
    SAKE_USING_TYPEDEF( typename traits_, traversal );
#define assert_is_same( type ) \
    BOOST_MPL_ASSERT((boost::is_same< typename relax_traits::type, type >));
    assert_is_same( value_type )
    assert_is_same( reference )
    assert_is_same( difference_type )
    assert_is_same( traversal )
#undef assert_is_same
    SAKE_USING_TYPEDEF( typename traits_, introterminal );
    BOOST_MPL_ASSERT_NOT((
        boost::is_same< typename relax_traits::introterminal, introterminal >));
};

template< class I, class Introterminal >
struct IntroterminalRelax< I, Introterminal, I >
{ };

/*******************************************************************************
 * struct IntroterminalBase< I, Introterminal >
 ******************************************************************************/

// Null
template< class I >
struct IntroterminalBase< I, sake::null_introterminal_tag >
    : sake::iterator::concepts::private_::Base<I>,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::null_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::null_introterminal_tag
    >::value));
};

// BeginDetect
template< class I >
struct IntroterminalBase< I, sake::begin_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_detect_introterminal_tag >,
      sake::concepts::EqualityComparable< I, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_introterminal_tag
    >::value));
};

// BeginAccess
template< class I >
struct IntroterminalBase< I, sake::begin_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::null_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_detect_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalDifference<
          I, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_introterminal_tag
    >::value));
};

// EndDetect
template< class I >
struct IntroterminalBase< I, sake::end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::null_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::end_detect_introterminal_tag >,
      sake::concepts::EqualityComparable< I, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::end_detect_introterminal_tag
    >::value));
};

// EndAccess
template< class I >
struct IntroterminalBase< I, sake::end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::end_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::null_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::end_detect_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalDifference<
          I, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::end_access_introterminal_tag
    >::value));
};

// BeginDetect + EndDetect
template< class I >
struct IntroterminalBase< I, sake::begin_detect_end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_detect_end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_end_detect_introterminal_tag
    >::value));
};

// BeginAccess + EndDetect
template< class I >
struct IntroterminalBase< I, sake::begin_access_end_detect_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_end_detect_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_access_end_detect_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_end_detect_introterminal_tag
    >::value));
};

// BeginDetect + EndAccess
template< class I >
struct IntroterminalBase< I, sake::begin_detect_end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::end_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_detect_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_detect_end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_detect_end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_detect_end_access_introterminal_tag
    >::value));
};

// BeginAccess + EndAccess
template< class I >
struct IntroterminalBase< I, sake::begin_access_end_access_introterminal_tag >
    : sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_access_end_detect_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalBase<
          I, sake::begin_detect_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_end_access_introterminal_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_end_access_introterminal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroterminalAt<
          I, sake::begin_access_end_access_introterminal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroterminalRelax<
          I, sake::begin_access_end_access_introterminal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_end_access_introterminal_tag
    >::value));
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROTERMINAL_BASE_HPP
