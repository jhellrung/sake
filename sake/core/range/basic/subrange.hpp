/*******************************************************************************
 * sake/core/range/basic/subrange.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::basic::subrange<I>
 * struct range::basic::subrange<I,N>
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
#include <sake/core/iterator/adapt_introterminal.hpp>
#include <sake/core/iterator/adapt_introterminal_fwd.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>
#include <sake/core/math/advance.hpp>
#include <sake/core/math/next.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/range/basic/fwd.hpp>
#include <sake/core/range/basic/private/subrange/impl_base.hpp>
#include <sake/core/range/basic/private/subrange/impl_base_c.hpp>
#include <sake/core/range/basic/private/subrange/range_constructor_enable.hpp>
#include <sake/core/range/basic/private/subrange/tags.hpp>
#include <sake/core/range/core_access.hpp>
#include <sake/core/range/private/introterminal_of_begin_end.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/using_typedef.hpp>

namespace sake
{

namespace range
{

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
    typedef typename sake::iterator_introterminal<I>::type base_introterminal;
public:

    typedef boost::true_type is_adapt_by_value_tag;

    SAKE_MEMBERWISE_MEM_FUN(
        typename subrange,
        ( default_constructor )( swap ),
        (( impl_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( impl_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( impl_base_ )) )

    template< class J >
    explicit subrange(J const & j,
        typename boost::enable_if_c< boost_ext::mpl::and3<
            boost::mpl::not_< boost::is_same<
                base_introterminal, sake::null_introterminal_tag > >,
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

    template< class This, class Introterminal >
    struct derived_iterator_with_of
        : sake::iterator::adapt_introterminal<
              I, Introterminal, base_introterminal >
    { };

    template< class T, class Introterminal >
    static typename impl_base_::template
        iterator_with< Introterminal >::type
    derived_iter_at(subrange const & this_, T const & x, Introterminal)
    { return this_.derived_iter_at_(x, Introterminal()); }

    using impl_base_::derived_iter_at_;

    template< class T, class Introterminal >
    typename boost::lazy_disable_if_c<
        boost_ext::is_convertible< base_introterminal, Introterminal >::value,
        typename impl_base_::template iterator_with< Introterminal >
    >::type
    derived_iter_at_(T const & x, Introterminal) const
    {
        return typename impl_base_::template
            iterator_with< Introterminal >::type(*this, x);
    }

    template< class This, class Begin, class End >
    class derived_subrange_with_of
    {
        typedef typename sake::introterminal_meet<
            typename sake::range::private_::
                introterminal_of_begin_end< Begin, End >::type,
            base_introterminal
        >::type introterminal;
    public:
        typedef sake::range::basic::subrange<
            typename sake::iterator::adapt_introterminal<
                I, introterminal, introterminal >::type > type;
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
    : public subrange_private::impl_base_c<I,N>
{
    typedef subrange_private::impl_base_c<I,N> impl_base_c_;
    SAKE_USING_TYPEDEF( typename impl_base_c_, facade_ );
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
        (( impl_base_c_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( impl_base_c_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( impl_base_c_ )) )

    template< class J >
    explicit subrange(J const & j,
        typename boost::enable_if_c< boost_ext::mpl::and2<
            boost_ext::not_is_base_of_sans_qualifiers< subrange, J >,
            boost_ext::is_convertible<J,I>
        >::value >::type* = 0)
        : impl_base_c_(subrange_private::iterator_tag(), j)
    { }

private:
    using impl_base_c_::m_begin;

    friend class sake::range::core_access;

    template< class This, class Introterminal >
    struct derived_iterator_with_of
        : sake::iterator::adapt_introterminal< I, Introterminal >
    { };

    template< class T, class Introterminal >
    static typename impl_base_c_::template iterator_with< Introterminal >::type
    derived_iter_at(subrange const & this_, T const & x, Introterminal)
    { return this_.derived_iter_at_(x, Introterminal()); }

    using impl_base_c_::derived_iter_at_;

    I derived_iter_at_(sake::begin_tag, sake::null_introterminal_tag) const
    { return m_begin; }
    template< class T >
    I derived_iter_at_(T const & x, sake::null_introterminal_tag) const
    { return x; }

    template< class T, class Introterminal >
    typename impl_base_c_::template iterator_with< Introterminal >::type
    derived_iter_at_(T const & x, Introterminal) const
    {
        return typename impl_base_c_::template
            iterator_with< Introterminal >::type(*this, x);
    }

    template< class This, class Begin, class End >
    struct derived_subrange_with_of
    { typedef sake::range::basic::subrange<I> type; };

    template< class Begin, class End >
    static sake::range::basic::subrange<I>
    derived_sub(subrange const & this_, Begin const & b, End const & e)
    { return sake::range::basic::subrange<I>(this_, b, e); }

    using impl_base_c_::derived_at;

    static reference derived_at(subrange const & this_, sake::begin_tag)
    { return *this_.m_begin; }
    template< class T >
    static reference derived_at(subrange const & this_, T const i)
    { return this_.m_begin[static_cast< difference_type >(i)]; }
};

} // namespace basic

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_BASIC_SUBRANGE_HPP
