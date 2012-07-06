/*******************************************************************************
 * sake/core/iterator/adaptors/introversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_INTROVERSAL_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_INTROVERSAL_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>

#include <sake/core/iterator/adapt_introversal.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/private/introversal/assign_enable.hpp>
#include <sake/core/iterator/adaptors/private/introversal/impl_base.hpp>
#include <sake/core/iterator/adaptors/private/introversal/implicit_constructor_enable.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
#include <sake/core/memberwise/type_trait_tag.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/utility/conversion_operators/core_access.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace iterator
{

namespace adaptors
{

template<
    class I,
    class Tags /*= boost::mpl::set0<> */,
    class IntroversalMask /*= sake::null_introversal_tag*/
>
class introversal
    : public introversal_private::impl_base< I, Tags, IntroversalMask >
{
    typedef introversal_private::impl_base<
        I, Tags, IntroversalMask > impl_base_;
public:

    SAKE_MEMBERWISE_MEM_FUN(
        typename introversal,
        ( default_constructor )( swap ),
        (( impl_base_ ))
    )
    SAKE_MEMBERWISE_TYPEDEF_TYPE_TRAIT_TAG(
        (( impl_base_ )),
        ( has_copy_constructor )
        ( has_nothrow_copy_constructor )
        ( has_nothrow_copy_assign )
    )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introversal(R&& r, sake::begin_tag)
        : impl_base_(r, sake::_begin)
    { }
    template< class R >
    introversal(R&& r, sake::end_tag)
        : impl_base_(r, sake::_end)
    { }
    template< class R, class J >
    introversal(R&& r, J const & j)
        : impl_base_(r,j)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introversal(R& r, sake::begin_tag)
        : impl_base_(SAKE_AS_LVALUE(r), sake::_begin)
    { }
    template< class R >
    introversal(R& r, sake::end_tag)
        : impl_base_(SAKE_AS_LVALUE(r), sake::_end)
    { }
    template< class R, class J >
    introversal(R& r, J const & j)
        : impl_base_(SAKE_AS_LVALUE(r), j)
    { }

    template< class R >
    introversal(R const & r, sake::begin_tag)
        : impl_base_(r, sake::_begin)
    { }
    template< class R >
    introversal(R const & r, sake::end_tag)
        : impl_base_(r, sake::_end)
    { }
    template< class R, class J >
    introversal(R const & r, J const & j)
        : impl_base_(r,j)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class J >
    struct implicit_constructor_enabler
        : introversal_private::implicit_constructor_enabler< introversal, J >
    { };
public:
    template< class J >
    introversal(J const & j,
        typename implicit_constructor_enabler<J>::type* = 0)
        : impl_base_(j)
    { }

    template< class J, class Begin, class End >
    introversal(J const & j, Begin const & begin_, End const & end_)
        : impl_base_(j, begin_, end_)
    { }

private:
    template< class J >
    struct assign_enabler
        : introversal_private::assign_enabler< introversal, J >
    { };
public:
    template< class J >
    typename assign_enabler<J>::type
    operator=(J const & j)
    { return impl_base_::operator=(j).derived(); }

private:
    using impl_base_::protected_base;

    friend class sake::iterator::core_access;

    template< class Introversal >
    struct derived_relax
    {
        typedef typename sake::iterator::adapt_introversal<
            I, Introversal, IntroversalMask >::type type;
    };

    friend class sake::conversion_operators::core_access;

    template< class T >
    T derived_convert(sake::type_tag<T>) const
    { return impl_base_::base(); }
};

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_INTROVERSAL_HPP
