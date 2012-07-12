/*******************************************************************************
 * sake/core/iterator/private/facade/begin_introterminal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROTERMINAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROTERMINAL_BASE_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/if.hpp>
#include <sake/boost_ext/type_traits/is_base_of_sans_qualifiers.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/iterator/facade_fwd.hpp>
#include <sake/core/iterator/private/facade/end_introterminal_base.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/swap.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

namespace iterator
{

namespace facade_adl
{

namespace private_
{

template< class Params >
struct begin_introterminal_base_index
{
    typedef private_::traits< Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_introterminal );
    static int const value = boost_ext::mpl::
    eval_if<
        boost_ext::is_convertible<
            iterator_introterminal,
            sake::begin_access_introterminal_tag
        >,
        boost::mpl::if_<
            typename traits_::difference_enable,
            sake::int_tag<3>,
            sake::int_tag<2>
        >
    >::type::template
    else_if<
        boost_ext::is_convertible<
            iterator_introterminal,
            sake::begin_detect_introterminal_tag
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type::value;
};

template< class Derived, class Params >
class begin_introterminal_base< Derived, Params, 0 >
    : public private_::end_introterminal_base< Derived, Params >
{
    typedef private_::end_introterminal_base<
        Derived, Params > end_introterminal_base_;
public:
    SAKE_MEMBERWISE_SWAP(
        typename begin_introterminal_base, (( end_introterminal_base_ )) )
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introterminal_base,
        (( end_introterminal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( end_introterminal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( end_introterminal_base_ )) )

    template< class T >
    explicit begin_introterminal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            begin_introterminal_base, T >::value >::type* = 0)
        : end_introterminal_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class begin_introterminal_base< Derived, Params, 1 >
    : public private_::end_introterminal_base< Derived, Params >
{
    typedef private_::end_introterminal_base<
        Derived, Params > end_introterminal_base_;
protected:
    using end_introterminal_base_::derived;
public:

    SAKE_MEMBERWISE_SWAP(
        typename begin_introterminal_base, (( end_introterminal_base_ )) )

    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, bool >::type
    operator==(Derived_ const & this_, sake::begin_tag)
    { return this_.private_equal_begin(); }
    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, bool >::type
    operator==(sake::begin_tag, Derived_ const & this_)
    { return this_.private_equal_begin(); }

    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, bool >::type
    operator!=(Derived_ const & this_, sake::begin_tag)
    { return !this_.private_equal_begin(); }
    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, bool >::type
    operator!=(sake::begin_tag, Derived_ const & this_)
    { return !this_.private_equal_begin(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introterminal_base,
        (( end_introterminal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( end_introterminal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( end_introterminal_base_ )) )

    template< class T >
    explicit begin_introterminal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            begin_introterminal_base, T >::value >::type* = 0)
        : end_introterminal_base_(sake::forward<T>(x))
    { }

private:
    bool private_equal_begin() const
    { return sake::iterator::core_access::equal(derived(), sake::_begin); }
};

template< class Derived, class Params >
class begin_introterminal_base< Derived, Params, 2 >
    : public private_::begin_introterminal_base< Derived, Params, 1 >
{
    typedef private_::begin_introterminal_base<
        Derived, Params, 1 > begin_introterminal_base_;
protected:
    using begin_introterminal_base_::derived;
public:

    SAKE_MEMBERWISE_SWAP(
        typename begin_introterminal_base, (( begin_introterminal_base_ )) )

    void begin_ip()
    { sake::iterator::core_access::at_ip(derived(), sake::_begin); }

    template< class Introterminal >
    typename begin_introterminal_base_::template relax< Introterminal >::type
    begin(Introterminal) const
    {
        return sake::iterator::core_access::at(
            derived(), sake::_begin, Introterminal());
    }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introterminal_base,
        (( begin_introterminal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( begin_introterminal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( begin_introterminal_base_ )) )

    template< class T >
    explicit begin_introterminal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            begin_introterminal_base, T >::value >::type* = 0)
        : begin_introterminal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    using begin_introterminal_base_::derived_equal;
    bool derived_equal(sake::begin_tag) const
    { return derived() == derived().at(sake::_begin); }
};

template< class Derived, class Params >
class begin_introterminal_base< Derived, Params, 3 >
    : public private_::begin_introterminal_base< Derived, Params, 2 >
{
    typedef private_::begin_introterminal_base<
        Derived, Params, 2 > begin_introterminal_base_;
protected:
    using begin_introterminal_base_::derived;
public:
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, difference_type );

    SAKE_MEMBERWISE_SWAP(
        typename begin_introterminal_base, (( begin_introterminal_base_ )) )

    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, difference_type >::type
    operator-(Derived_ const & this_, sake::begin_tag)
    { return this_.private_difference_begin(); }
    template< class Derived_ >
    inline friend typename boost::enable_if_c<
        boost::is_same< Derived_, Derived >::value, difference_type >::type
    operator-(sake::begin_tag, Derived_ const & this_)
    { return -this_.private_difference_begin(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename begin_introterminal_base,
        (( begin_introterminal_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( begin_introterminal_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( begin_introterminal_base_ )) )

    template< class T >
    explicit begin_introterminal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            begin_introterminal_base, T >::value >::type* = 0)
        : begin_introterminal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    using begin_introterminal_base_::derived_difference;
    difference_type derived_difference(sake::begin_tag) const
    { return derived() - derived().at(sake::_begin); }

private:
    difference_type private_difference_begin() const
    { return sake::iterator::core_access::difference(derived(), sake::_begin); }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_BEGIN_INTROTERMINAL_BASE_HPP
