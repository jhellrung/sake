/*******************************************************************************
 * sake/core/iterator/private/facade/end_introversal_base.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP

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
#include <sake/core/iterator/private/facade/common_base.hpp>
#include <sake/core/iterator/private/facade/traits.hpp>
#include <sake/core/memberwise/default_constructor.hpp>
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
struct end_introversal_base_index
{
    typedef private_::traits< Params > traits_;
    SAKE_USING_TYPEDEF( typename traits_, iterator_introversal );
    static int const value = boost_ext::mpl::
    eval_if<
        boost_ext::is_convertible<
            iterator_introversal,
            sake::end_access_introversal_tag
        >,
        boost::mpl::if_<
            typename traits_::difference_enable,
            sake::int_tag<3>,
            sake::int_tag<2>
        >
    >::type::template
    else_if<
        boost_ext::is_convertible<
            iterator_introversal,
            sake::end_detect_introversal_tag
        >,
        sake::int_tag<1>
    >::type::template
    else_<
        sake::int_tag<0>
    >::type::value;
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 0 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( common_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            end_introversal_base, T >::value >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 1 >
    : public private_::common_base< Derived, Params >
{
    typedef private_::common_base< Derived, Params > common_base_;
protected:
    using common_base_::derived;
public:

    inline friend
    bool operator==(Derived const & this_, sake::end_tag)
    { return this_.private_operator_equal_end(); }
    inline friend
    bool operator==(sake::end_tag, Derived const & this_)
    { return this_.private_operator_equal_end(); }

    inline friend
    bool operator!=(Derived const & this_, sake::end_tag)
    { return !this_.private_operator_equal_end(); }
    inline friend
    bool operator!=(sake::end_tag, Derived const & this_)
    { return !this_.private_operator_equal_end(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( common_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            end_introversal_base, T >::value >::type* = 0)
        : common_base_(sake::forward<T>(x))
    { }

private:
    bool private_operator_equal_end() const
    { return sake::iterator::core_access::equal_end(derived()); }
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 2 >
    : public private_::end_introversal_base< Derived, Params, 1 >
{
    typedef private_::end_introversal_base<
        Derived, Params, 1 > end_introversal_base_;
protected:
    using end_introversal_base_::derived;
public:

    template< class Introversal = sake::null_introversal_tag >
    struct relax
        : end_introversal_base_::template relax< Introversal >
    { };

    using end_introversal_base_::at_ip;
    using end_introversal_base_::at;

    Derived&
    at_ip(sake::end_tag)
    {
        sake::iterator::core_access::at_end_ip(derived());
        return derived();
    }

    template< class EndTag >
    typename boost::lazy_enable_if_c<
        boost::is_same< EndTag, sake::end_tag >::value,
        relax< sake::null_introversal_tag >
    >::type
    at(EndTag)
    { return at(sake::_end, sake::null_introversal_tag()); }

    template< class Introversal >
    typename relax< Introversal >::type
    at(sake::end_tag, Introversal) const
    { return sake::iterator::core_access::at_end(derived(), Introversal()); }

    Derived&
    end_ip()
    { return at_ip(sake::_end); }

    template< class Introversal >
    typename relax< Introversal >::type
    end(Introversal) const
    { return at(sake::_end, Introversal()); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( end_introversal_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            end_introversal_base, T >::value >::type* = 0)
        : end_introversal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    bool derived_equal_end() const
    { return derived() == end(); }

    template< class Introversal >
    typename relax< Introversal >::type
    derived_at_end(Introversal) const
    {
        Derived result(derived());
        return result.end_ip();
    }
};

template< class Derived, class Params >
class end_introversal_base< Derived, Params, 3 >
    : public private_::end_introversal_base< Derived, Params, 2 >
{
    typedef private_::end_introversal_base<
        Derived, Params, 2 > end_introversal_base_;
protected:
    using end_introversal_base_::derived;
public:
    SAKE_USING_TYPEDEF( typename private_::traits< Params >, difference_type );

    using end_introversal_base_::end;

    inline friend
    difference_type operator-(Derived const & this_, sake::end_tag)
    { return this_.private_operator_minus_end(); }
    inline friend
    difference_type operator-(sake::end_tag, Derived const & this_)
    { return -this_.private_operator_minus_end(); }

protected:
    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename end_introversal_base,
        (( end_introversal_base_ ))
    )

    template< class T >
    explicit end_introversal_base(SAKE_FWD2_REF( T ) x,
        typename boost::disable_if_c< boost_ext::is_base_of_sans_qualifiers<
            end_introversal_base, T >::value >::type* = 0)
        : end_introversal_base_(sake::forward<T>(x))
    { }

    friend class sake::iterator::core_access;

    difference_type derived_difference_end() const
    { return derived() - end(); }

private:
    difference_type private_operator_minus_end() const
    { return sake::iterator::core_access::difference_end(derived()); }
};

} // namespace private_

} // namespace facade_adl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_FACADE_END_INTROVERSAL_BASE_HPP
