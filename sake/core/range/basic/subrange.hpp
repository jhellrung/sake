/*******************************************************************************
 * sake/core/range/basic/subrange.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::basic::subrange<I>
 * struct range::basic::subrange<I,N>
 *
 * range::sub(Begin b, End e) -> range::result_of::sub< Begin, End >::type
 * range::sub_c<N>(I i) -> range::result_of::sub_c<I,N>::type
 *
 * struct range::functional::sub
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_BASIC_SUBRANGE_HPP
#define SAKE_CORE_RANGE_BASIC_SUBRANGE_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/iterator/adapt_introversal.hpp>
#include <sake/core/iterator/adapt_introversal_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/math/advance.hpp>
#include <sake/core/math/next.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/range/basic/fwd.hpp>
#include <sake/core/range/basic/private/subrange/impl_base.hpp>
#include <sake/core/range/basic/private/subrange/range_constructor_enable.hpp>
#include <sake/core/range/basic/private/subrange/result_of.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/private/introversal_from_begin_end.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

namespace functional
{

struct sub
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::result_of::sub, 2 )

    template< class Begin, class End >
    typename sake::range::result_of::sub< Begin, End >::type
    operator()(Begin const & b, End const & e) const
    { return typename sake::range::result_of::sub< Begin, End >::type(b,e); }
};

} // namespace functional

static sake::range::functional::sub const sub = { };

template< std::size_t N, class I >
inline typename sake::range::result_of::sub_c<I,N>::type
sub_c(I const & i)
{ return typename sake::range::result_of::sub_c<I,N>::type(i); }

namespace basic
{

/*******************************************************************************
 * class range::basic::subrange<I>
 ******************************************************************************/

template< class I >
class subrange< I, void >
    : public subrange_private::impl_base<I>
{
    typedef subrange_private::impl_base<I> impl_base_;
    typedef typename sake::iterator_introversal<I>::type base_introversal;
public:

    typedef boost::true_type is_adapt_by_value_tag;

    SAKE_MEMBERWISE_MEM_FUN(
        typename subrange,
        ( default_constructor )( swap ),
        (( impl_base_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( impl_base_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    template< class J >
    explicit subrange(J const & j,
        typename boost::enable_if_c< boost_ext::mpl::and3<
            boost::mpl::not_< boost::is_same<
                base_introversal, sake::null_introversal_tag > >,
            boost_ext::not_is_base_of_sans_qualifiers< subrange, J >,
            boost_ext::is_convertible<J,I>
        >::value >::type* = 0)
        : impl_base_(subrange_private::iterator_tag(), j)
    { }

    template< class Begin, class End >
    subrange(Begin const & b, End const & e)
        : impl_base_(b, e)
    { }

private:
    template< class R >
    struct range_constructor_enabler
        : subrange_private::range_constructor_enabler< subrange, R >
    { };
public:

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    explicit subrange(R&& r,
        typename range_constructor_enabler<R>::type* = 0)
        : impl_base_(subrange_private::range_tag(), r)
    { }
    template< class R, class Begin, class End >
    subrange(R&& r, Begin const & b, End const & e)
        : impl_base_(r, b, e)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    explicit subrange(R& r,
        typename range_constructor_enabler< R& >::type* = 0)
        : impl_base_(subrange_private::range_tag(), SAKE_AS_LVALUE(r))
    { }
    template< class R, class Begin, class End >
    subrange(R& r, Begin const & b, End const & e)
        : impl_base_(SAKE_AS_LVALUE(r), b, e)
    { }

    template< class R >
    explicit subrange(R const & r,
        typename range_constructor_enabler< R const & >::type* = 0)
        : impl_base_(subrange_private::range_tag(), r)
    { }
    template< class R, class Begin, class End >
    subrange(R const & r, Begin const & b, End const & e)
        : impl_base_(r, b, e)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    friend class sake::range::core_access;

    template< class This, class Introversal >
    struct derived_iterator_with_of
        : sake::iterator::adapt_introversal< I, Introversal, base_introversal >
    { };

    template< class T, class Introversal >
    static typename impl_base_::template
        iterator_with< Introversal >::type
    derived_iter_at(subrange const & this_, T const & x, Introversal)
    { return this_.derived_iter_at_(x, Introversal()); }

    using impl_base_::derived_iter_at_;

    template< class T, class Introversal >
    typename boost::lazy_disable_if_c<
        boost_ext::is_convertible< base_introversal, Introversal >::value,
        typename impl_base_::template iterator_with< Introversal >
    >::type
    derived_iter_at_(T const & x, Introversal) const
    {
        return typename impl_base_::template
            iterator_with< Introversal >::type(*this, x);
    }

    template< class This, class Begin, class End >
    class derived_subrange_with_of
    {
        typedef typename sake::introversal_meet<
            typename sake::range::private_::
                introversal_from_begin_end< Begin, End >::type,
            base_introversal
        >::type introversal;
    public:
        typedef sake::range::basic::subrange<
            typename sake::iterator::adapt_introversal<
                I, introversal, introversal >::type > type;
    };

    template< class Begin, class End >
    static typename impl_base_::template subrange_with< Begin, End >::type
    derived_sub(subrange const & this_, Begin const & b, End const & e)
    {
        return typename impl_base_::template
            subrange_with< Begin, End >::type(this_, b, e);
    }
};

/*******************************************************************************
 * class range::basic::subrange<I,N>
 ******************************************************************************/

template< class I, class N >
class subrange
    : public subrange_private::traits<I,N>::facade_
{
    typedef subrange_private::traits<I,N> traits_;
    SAKE_USING_TYPEDEF( typename traits_, facade_ );
    BOOST_STATIC_ASSERT((boost::is_same<
        typename sake::iterator_introversal<I>::type,
        sake::null_introversal_tag
    >::value));
    BOOST_STATIC_ASSERT((N::value >= 0));
public:
    SAKE_USING_TYPEDEF( typename facade_, reference );
    SAKE_USING_TYPEDEF( typename facade_, difference_type );
    SAKE_USING_TYPEDEF( typename facade_, size_type );

    static std::size_t const static_size = N::value;

    typedef boost::true_type is_adapt_by_value_tag;

    SAKE_MEMBERWISE_MEM_FUN(
        typename subrange,
        ( default_constructor )( swap ),
        (( facade_ )) (( I )( m_begin ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( facade_ )) (( I )( m_begin )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

    template< class J >
    explicit subrange(J const & j,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            boost_ext::not_is_base_of_sans_qualifiers< subrange, J >,
            boost_ext::is_convertible<J,I>
        >::value >::type* = 0)
        : m_begin(j)
    { }

    template< class J, class M >
    subrange(J const & j, M,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            boost_ext::is_convertible<J,I>,
            sake::has_isc_value< M,
                typename boost_ext::mpl::curry_quote2<
                    boost::mpl::equal_to >::apply<N>::type >
        >::value >::type* = 0)
        : m_begin(j)
    { }

    void advance(difference_type const n)
    { sake::advance(m_begin, n); }

private:
    friend class sake::range::core_access;

    template< class This, class Introversal >
    struct derived_iterator_with_of
        : sake::iterator::adapt_introversal< I, Introversal >
    { };

    template< class T, class Introversal >
    static typename facade_::template iterator_with< Introversal >::type
    derived_iter_at(subrange const & this_, T const & x, Introversal)
    { return this_.derived_iter_at(x, Introversal()); }

    I derived_iter_at(sake::begin_tag, sake::null_introversal_tag) const
    { return m_begin; }
    I derived_iter_at(sake::end_tag, sake::null_introversal_tag) const
    { return sake::next_c< N::value >(m_begin); }
    template< class T >
    I derived_iter_at(T const & x, sake::null_introversal_tag) const
    { return x; }

    template< class T, class Introversal >
    typename facade_::template iterator_with< Introversal >::type
    derived_iter_at(T const & x, Introversal) const
    {
        return typename facade_::template
            iterator_with< Introversal >::type(*this, x);
    }

    template< class This, class Begin, class End >
    struct derived_subrange_with_of
    { typedef sake::range::basic::subrange<I> type; };

    template< class Begin, class End >
    static sake::range::basic::subrange<I>
    derived_sub(subrange const & this_, Begin const & b, End const & e)
    { return sake::range::basic::subrange<I>(this_, b, e); }

    static reference derived_at(subrange const & this_, sake::begin_tag)
    { return *this_.m_begin; }
    static reference derived_at(subrange const & this_, sake::end_tag)
    { return *sake::next_c< N::value - 1 >(this_.m_begin); }
    template< class T >
    static reference derived_at(subrange const & this_, T const i)
    { return this_.m_begin[static_cast< difference_type >(i)]; }

    I m_begin;
};

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_SUBRANGE_HPP
