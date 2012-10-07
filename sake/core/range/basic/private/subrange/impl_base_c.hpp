/*******************************************************************************
 * sake/core/range/basic/private/subrange/impl_base_c.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_STATIC_SIZE_HPP
#define SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_STATIC_SIZE_HPP

#include <cstddef>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/math/advance.hpp>
#include <sake/core/math/next.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/range/basic/private/subrange/traits.hpp>
#include <sake/core/utility/int_tag.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace basic
{

namespace subrange_private
{

template< class I >
struct impl_base_c_index
{
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introterminal<I>::type,
        sake::null_introterminal_tag
    >::value));
    typedef typename sake::iterator_traversal<I>::type traversal;
    static int const value = boost_ext::mpl::
    if_ <
        boost_ext::is_convertible<
            traversal,
            boost::random_access_traversal_tag
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<1>
    >::type::value;
};

template< class I, class N, int = impl_base_c_index<I>::value >
class impl_base_c;

/*******************************************************************************
 * class range::basic::subrange_private::impl_base_c<I,N,0>
 ******************************************************************************/

template< class I, class N >
class impl_base_c<I,N,0>
    : public subrange_private::traits<I,N>::facade_
{
    typedef subrange_private::traits<I,N> traits;
    SAKE_USING_TYPEDEF( typename traits, facade_ );
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base_c,
        (( facade_ ))
        (( I )( m_begin )) (( I )( m_end ))
    )

    void advance(difference_type const n)
    {
        sake::advance(m_end, n);
        sake::advance(m_begin, n);
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base_c,
        (( facade_ ))
        (( I )( m_begin )) (( I )( m_end ))
    )
    SAKE_MEMBERWISE_COPY_TAGS(
        (( facade_ ))
        (( I )( m_begin )) (( I )( m_end ))
    )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS(
        (( facade_ ))
        (( I )( m_begin )) (( I )( m_end ))
    )

    template< class J >
    impl_base_c(subrange_private::iterator_tag, J const & j)
        : m_begin(j),
          m_end(sake::next_c< N::value >(j))
    { }

    I derived_iter_at_(sake::end_tag, sake::null_introterminal_tag) const
    { return m_end; }

    friend class sake::range::core_access;

    static reference derived_at(impl_base_c const & this_, sake::end_tag)
    { return *sake::prior(this_.m_end); }

    I m_begin;

private:
    I m_end;
};

/*******************************************************************************
 * class range::basic::subrange_private::impl_base_c<I,N,1>
 ******************************************************************************/

template< class I, class N >
class impl_base_c<I,N,1>
    : public subrange_private::traits<I,N>::facade_
{
    typedef subrange_private::traits<I,N> traits;
    SAKE_USING_TYPEDEF( typename traits, facade_ );
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base_c,
        (( facade_ ))
        (( I )( m_begin ))
    )

    void advance(difference_type const n)
    { sake::advance(m_begin, n); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base_c,
        (( facade_ ))
        (( I )( m_begin ))
    )
    SAKE_MEMBERWISE_COPY_TAGS(
        (( facade_ ))
        (( I )( m_begin ))
    )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS(
        (( facade_ ))
        (( I )( m_begin ))
    )

    template< class J >
    impl_base_c(subrange_private::iterator_tag, J const & j)
        : m_begin(j)
    { }

    I derived_iter_at_(sake::end_tag, sake::null_introterminal_tag) const
    { return sake::next_c< N::value >(m_begin); }

    friend class sake::range::core_access;

    static reference derived_at(impl_base_c const & this_, sake::end_tag)
    { return *sake::next_c< N::value - 1 >(m_begin); }

    I m_begin;
};

} // namespace subrange_private

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_STATIC_SIZE_HPP
