/*******************************************************************************
 * sake/core/range/basic/private/subrange/impl_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_HPP
#define SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/at.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/distance.hpp>
#include <sake/core/iterator/distance_fwd.hpp>
#include <sake/core/iterator/private/adaptor/as_convertible_relax.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/math/advance.hpp>
#include <sake/core/math/prior.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/basic/private/subrange/tags.hpp>
#include <sake/core/range/basic/private/subrange/traits.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
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
struct impl_base_index
{
    typedef typename sake::iterator_introterminal<I>::type introterminal;
    static bool const implicit_begin = boost_ext::is_convertible<
        introterminal, sake::begin_access_introterminal_tag >::value;
    static bool const implicit_end = boost_ext::is_convertible<
        introterminal, sake::end_access_introterminal_tag >::value;
    static int const value = implicit_begin + 2 * implicit_end;
};

template< class I, int = impl_base_index<I>::value >
class impl_base;

/*******************************************************************************
 * class range::basic::subrange_private::impl_base<I,0>
 ******************************************************************************/

template< class I >
class impl_base<I,0>
    : public subrange_private::traits<I>::facade_
{
    typedef subrange_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introterminal<I>::type,
        sake::null_introterminal_tag
    >::value));
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    SAKE_USING_TYPEDEF( typename facade_, size_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base,
        (( facade_ ))
        (( I )( m_begin )) (( I )( m_end ))
    )

    void increment(sake::begin_tag)
    { ++m_begin; }
    void decrement(sake::begin_tag)
    { --m_begin; }
    template< class T >
    typename boost::enable_if_c< boost::is_integral<T>::value >::type
    advance(sake::begin_tag, T const n)
    { sake::advance(m_begin, n); }

    void increment(sake::end_tag)
    { ++m_end; }
    void decrement(sake::end_tag)
    { --m_end; }
    template< class T >
    typename boost::enable_if_c< boost::is_integral<T>::value >::type
    advance(sake::end_tag, T const n)
    { sake::advance(m_end, n); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
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

    template< class Begin, class End >
    impl_base(Begin const & b, End const & e,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            boost_ext::is_convertible< Begin const &, I >,
            boost_ext::is_convertible< End const &, I >
        >::value >::type* = 0)
        : m_begin(b),
          m_end(e)
    { }

    template< class R >
    impl_base(subrange_private::range_tag, R& r)
        : m_begin(sake::range_traits<R>::begin(r)),
          m_end(sake::range_traits<R>::end(r))
    { }

    template< class R, class Begin, class End >
    impl_base(R& r, Begin const & b, End const & e)
        : m_begin(sake::range_traits<R>::iter_at(r,b)),
          m_end(sake::range_traits<R>::iter_at(r,e))
    { }

    I derived_iter_at_(sake::begin_tag, sake::null_introterminal_tag) const
    { return m_begin; }
    I derived_iter_at_(sake::end_tag, sake::null_introterminal_tag) const
    { return m_end; }
    template< class T >
    I derived_iter_at_(T const & x, sake::null_introterminal_tag) const
    { return x; }

    friend class sake::range::core_access;

    static reference derived_at(impl_base const & this_, sake::begin_tag)
    { return *this_.m_begin; }
    static reference derived_at(impl_base const & this_, sake::end_tag)
    { return *sake::prior(this_.m_end); }
    template< class T >
    static reference derived_at(impl_base const & this_, T const i)
    { return this_.m_begin[static_cast< difference_type >(i)]; }

    bool derived_empty() const
    { return m_begin == m_end; }
    //static difference_type derived_distance() const;
    size_type derived_size() const
    { return static_cast< size_type >(m_end - m_begin); }

private:
    I m_begin;
    I m_end;
};

/*******************************************************************************
 * class range::basic::subrange_private::impl_base<I,1>
 ******************************************************************************/

template< class I >
class impl_base<I,1>
    : public subrange_private::traits<I>::facade_
{
    typedef subrange_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_introterminal_tag
    >::value));
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    SAKE_USING_TYPEDEF( typename facade_, size_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base,
        (( facade_ ))
        (( I )( m_end ))
    )

    void increment(sake::end_tag)
    { ++m_end; }
    void decrement(sake::end_tag)
    { --m_end; }
    template< class T >
    typename boost::enable_if_c< boost::is_integral<T>::value >::type
    advance(sake::end_tag, T const n)
    { sake::advance(m_end, n); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( facade_ ))
        (( I )( m_end ))
    )
    SAKE_MEMBERWISE_COPY_TAGS(
        (( facade_ ))
        (( I )( m_end ))
    )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS(
        (( facade_ ))
        (( I )( m_end ))
    )

    template< class J >
    explicit impl_base(subrange_private::iterator_tag, J const & j)
        : m_end(j)
    { }
    template< class End >
    impl_base(sake::begin_tag, End const & e)
        : m_end(e)
    { }

    template< class R >
    impl_base(subrange_private::range_tag, R& r)
        : m_end(sake::range_traits<R>::end(
              r, sake::begin_access_introterminal_tag()))
    { }

    template< class R, class End >
    impl_base(R& r, sake::begin_tag, End const & e)
        : m_end(sake::range_traits<R>::iter_at(
              r, e, sake::begin_access_introterminal_tag()))
    { }

private:
    template< class Introterminal >
    struct enable_if_is_convertible
        : boost::lazy_enable_if_c<
              boost_ext::is_convertible<
                  sake::begin_access_introterminal_tag, Introterminal >::value,
              typename facade_::template iterator_with< Introterminal >
          >
    { };
protected:
    template< class Introterminal >
    typename enable_if_is_convertible< Introterminal >::type
    derived_iter_at_(sake::end_tag, Introterminal) const
    { return m_end; }
    template< class T, class Introterminal >
    typename enable_if_is_convertible< Introterminal >::type
    derived_iter_at_(T const & x, Introterminal) const
    {
        return sake::iterator::at(
            m_end,
            sake::iterator::adaptor_private::as_convertible_relax<I>(x),
            Introterminal()
        );
    }

    friend class sake::range::core_access;

    using facade_::derived_at;
    static reference derived_at(impl_base const & this_, sake::end_tag)
    { return *sake::prior(this_.m_end); }

    bool derived_empty() const
    { return m_end == sake::_begin; }
    difference_type derived_distance() const
    { return sake::iterator::functional::distance()(sake::_begin, m_end); }
    size_type derived_size() const
    { return static_cast< size_type >(m_end - sake::_begin); }

private:
    I m_end;
};

/*******************************************************************************
 * class range::basic::subrange_private::impl_base<I,2>
 ******************************************************************************/

template< class I >
class impl_base<I,2>
    : public subrange_private::traits<I>::facade_
{
    typedef subrange_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introterminal<I>::type,
        sake::end_access_introterminal_tag
    >::value));
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    SAKE_USING_TYPEDEF( typename facade_, size_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base,
        (( facade_ ))
        (( I )( m_begin ))
    )

    void increment(sake::begin_tag)
    { ++m_begin; }
    void decrement(sake::begin_tag)
    { --m_begin; }
    template< class T >
    typename boost::enable_if_c< boost::is_integral<T>::value >::type
    advance(sake::begin_tag, T const n)
    { sake::advance(m_begin, n); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
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
    explicit impl_base(subrange_private::iterator_tag, J const & j)
        : m_begin(j)
    { }

    template< class Begin >
    impl_base(Begin const & b, sake::end_tag)
        : m_begin(b)
    { }

    template< class R >
    impl_base(subrange_private::range_tag, R& r)
        : m_begin(sake::range_traits<R>::begin(
              r, sake::end_access_introterminal_tag()))
    { }

    template< class R, class Begin >
    impl_base(R& r, Begin const & b, sake::end_tag)
        : m_begin(sake::range_traits<R>::iter_at(
              r, b, sake::end_access_introterminal_tag()))
    { }

private:
    template< class Introterminal >
    struct enable_if_is_convertible
        : boost::lazy_enable_if_c<
              boost_ext::is_convertible<
                  sake::end_access_introterminal_tag, Introterminal >::value,
              typename facade_::template iterator_with< Introterminal >
          >
    { };
protected:
    template< class Introterminal >
    typename enable_if_is_convertible< Introterminal >::type
    derived_iter_at_(sake::begin_tag, Introterminal) const
    { return m_begin; }
    template< class T, class Introterminal >
    typename enable_if_is_convertible< Introterminal >::type
    derived_iter_at_(T const & x, Introterminal) const
    {
        return sake::iterator::at(
            m_begin,
            sake::iterator::adaptor_private::as_convertible_relax<I>(x),
            Introterminal()
        );
    }

    friend class sake::range::core_access;

    using facade_::derived_at;
    static reference derived_at(impl_base const & this_, sake::begin_tag)
    { return *this_.m_begin; }
    template< class T >
    static reference derived_at(impl_base const & this_, T const i)
    { return this_.m_begin[static_cast< difference_type >(i)]; }

    bool derived_empty() const
    { return m_begin == sake::_end; }
    difference_type derived_distance() const
    { return sake::iterator::functional::distance()(m_begin, sake::_end); }
    size_type derived_size() const
    { return static_cast< size_type >(sake::_end - m_begin); }

private:
    I m_begin;
};

/*******************************************************************************
 * class range::basic::subrange_private::impl_base<I,3>
 ******************************************************************************/

template< class I >
class impl_base<I,3>
    : public subrange_private::traits<I>::facade_
{
    typedef subrange_private::traits<I> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introterminal<I>::type,
        sake::begin_access_end_access_introterminal_tag
    >::value));
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    SAKE_USING_TYPEDEF( typename facade_, size_type );

    SAKE_MEMBERWISE_SWAP(
        typename impl_base,
        (( facade_ ))
        (( I )( m_i ))
    )

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename impl_base,
        (( facade_ ))
        (( I )( m_i ))
    )
    SAKE_MEMBERWISE_COPY_TAGS(
        (( facade_ ))
        (( I )( m_i ))
    )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS(
        (( facade_ ))
        (( I )( m_i ))
    )

    template< class J >
    explicit impl_base(subrange_private::iterator_tag, J const & j)
        : m_i(j)
    { }

    template< class R >
    impl_base(subrange_private::range_tag, R& r)
        : m_i(sake::range_traits<R>::begin(
              r, sake::begin_access_end_access_introterminal_tag()))
    { }

    template< class R >
    impl_base(R& r, sake::begin_tag, sake::end_tag)
        : m_i(sake::range_traits<R>::begin(
              r, sake::begin_access_end_access_introterminal_tag()))
    { }

    template< class T, class Introterminal >
    typename facade_::template iterator_with< Introterminal >::type
    derived_iter_at_(T const & x, Introterminal) const
    {
        return sake::iterator::at(
            m_i,
            sake::iterator::adaptor_private::as_convertible_relax<I>(x),
            Introterminal()
        );
    }

    friend class sake::range::core_access;

private:
    I m_i;
};

} // namespace subrange_private

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_PRIVATE_SUBRANGE_IMPL_BASE_HPP
