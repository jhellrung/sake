/*******************************************************************************
 * sake/core/iterator/concepts/private/IntroversalBase.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROVERSAL_BASE_HPP

#include <boost/concept/usage.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/concepts/EqualityComparable.hpp>
#include <sake/core/config.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/Base.hpp>
#include <sake/core/iterator/traits.hpp>
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

template< class Introversal >
struct IntroversalBase_index;

template<
    class I,
    class Introversal = typename sake::iterator_introversal<I>::type,
    int = IntroversalBase_index< Introversal >::value
>
struct IntroversalBase;

/*******************************************************************************
 * struct IntroversalAt< I, Introversal, Tag = void >
 ******************************************************************************/

template< class I, class Introversal, class Tag = void >
struct IntroversalAt
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, introversal );

    typedef typename traits_::template relax< Introversal >::type relax_type;

    BOOST_CONCEPT_USAGE( IntroversalAt )
    {
        traits_::at_ip(i, Tag());
        assert_result<I>(traits_::at(ci, Tag(), introversal()));
        assert_result< relax_type >(traits_::at(ci, Tag(), Introversal()));
    }

private:
    I i;
    I const ci;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroversalAt )
};

template< class I, class Introversal >
struct IntroversalAt< I, Introversal, void >
{
private:
    typedef sake::iterator_traits<I> traits_;
public:
    SAKE_USING_TYPEDEF( typename traits_, introversal );
    BOOST_STATIC_ASSERT((
        boost_ext::is_convertible< introversal, Introversal >::value));

    typedef typename traits_::template relax< Introversal >::type relax_type;
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal< relax_type >::type,
        Introversal
    >::value));
    BOOST_STATIC_ASSERT((boost_ext::is_convertible< I, relax_type >::value));

    BOOST_CONCEPT_USAGE( IntroversalAt )
    {
        traits_::at_ip(i, ci);
        traits_::at_ip(i, cj);
        assert_result<I>(traits_::at(ci, cj, introversal()));
        assert_result< relax_type >(traits_::at(ci, cj, Introversal()));
    }

private:
    I i;
    I const ci;
    relax_type const cj;

    template< class T >
    void assert_result(T);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroversalAt )
};

/*******************************************************************************
 * struct IntroversalDifference< I, Tag >
 ******************************************************************************/

template<
    class I, class Tag,
    bool = boost_ext::is_convertible<
        typename sake::iterator_traversal<I>::type,
        boost::random_access_traversal_tag
    >::value
>
struct IntroversalDifference;

template< class I, class Tag >
struct IntroversalDifference< I, Tag, false >
{ };

template< class I, class Tag >
struct IntroversalDifference< I, Tag, true >
{
    typedef typename sake::iterator_difference<I>::type difference_type;

    BOOST_CONCEPT_USAGE( IntroversalDifference )
    {
        assert_result(i - Tag());
        assert_result(Tag() - i);
    }

private:
    I const i;

    void assert_result(difference_type);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( IntroversalDifference )
};

/*******************************************************************************
 * struct IntroversalRelax< I, Introversal >
 ******************************************************************************/

template<
    class I, class Introversal,
    class Relax = typename sake::iterator_relax< I, Introversal >::type
>
struct IntroversalRelax
    : sake::iterator::concepts::private_::IntroversalBase< Relax, Introversal >
{ };

template< class I, class Introversal >
struct IntroversalRelax< I, Introversal, I >
{ };

/*******************************************************************************
 * struct IntroversalBase< I, Introversal >
 ******************************************************************************/

template< class Introversal >
struct IntroversalBase_index
{
#define is_convertible_( x ) \
    boost_ext::is_convertible< Introversal, sake::x ## _introversal_tag >
    static int const begin_value = boost_ext::mpl::
         if_< is_convertible_( begin_access ),
              sake::int_tag<2> >::type::template
    else_if < is_convertible_( begin_detect ),
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
    static int const end_value = boost_ext::mpl::
         if_< is_convertible_( end_access ),
              sake::int_tag<2> >::type::template
    else_if < is_convertible_( end_detect ),
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
#undef is_convertible_
    static int const value = begin_value + 3 * end_value;
};

// Null
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 0 >
    : sake::iterator::concepts::private_::Base<I>,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::null_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::null_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::null_introversal_tag
    >::value));
};

// BeginDetect
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 1 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::null_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_detect_introversal_tag >,
      sake::concepts::EqualityComparable< I, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_detect_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_detect_introversal_tag
    >::value));
};

// BeginAccess
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 2 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::null_introversal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_detect_introversal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_introversal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalDifference<
          I, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_access_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_access_introversal_tag
    >::value));
};

// EndDetect
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 3 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::null_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::end_detect_introversal_tag >,
      sake::concepts::EqualityComparable< I, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::end_detect_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::end_detect_introversal_tag
    >::value));
};

// EndAccess
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 6 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::end_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::null_introversal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::end_detect_introversal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::end_access_introversal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalDifference<
          I, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::end_access_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::end_access_introversal_tag
    >::value));
};

// BeginDetect + EndDetect
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 4 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalBase<
          I, sake::end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_detect_end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_detect_end_detect_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_detect_end_detect_introversal_tag
    >::value));
};

// BeginAccess + EndDetect
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 5 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_end_detect_introversal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_access_end_detect_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_access_end_detect_introversal_tag
    >::value));
};

// BeginDetect + EndAccess
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 7 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalBase<
          I, sake::end_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_detect_end_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_detect_end_access_introversal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_detect_end_access_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_detect_end_access_introversal_tag
    >::value));
};

// BeginAccess + EndAccess
template< class I, class Introversal >
struct IntroversalBase< I, Introversal, 8 >
    : sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_access_end_detect_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalBase<
          I, sake::begin_detect_end_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_end_access_introversal_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_end_access_introversal_tag, sake::begin_tag >,
      sake::iterator::concepts::private_::IntroversalAt<
          I, sake::begin_access_end_access_introversal_tag, sake::end_tag >,
      sake::iterator::concepts::private_::IntroversalRelax<
          I, sake::begin_access_end_access_introversal_tag >
{
    BOOST_STATIC_ASSERT((boost_ext::is_convertible<
        typename sake::iterator_introversal<I>::type,
        sake::begin_access_end_access_introversal_tag
    >::value));
};

} // namespace private_

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_PRIVATE_INTROVERSAL_BASE_HPP
