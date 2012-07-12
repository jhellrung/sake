/*******************************************************************************
 * sake/core/iterator/adaptors/introterminal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_ADAPTORS_INTROTERMINAL_HPP
#define SAKE_CORE_ITERATOR_ADAPTORS_INTROTERMINAL_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>

#include <sake/core/iterator/adapt_introterminal.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/private/introterminal/assign_enable.hpp>
#include <sake/core/iterator/adaptors/private/introterminal/impl_base.hpp>
#include <sake/core/iterator/adaptors/private/introterminal/implicit_constructor_enable.hpp>
#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/concepts/Iterator.hpp>
#include <sake/core/iterator/core_access.hpp>
#include <sake/core/memberwise/copy_tags.hpp>
#include <sake/core/memberwise/destructor_tags.hpp>
#include <sake/core/memberwise/mem_fun.hpp>
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
    class Tags /*= boost::mpl::set0<>*/,
    class IntroterminalMask /*= sake::null_introterminal_tag*/
>
class introterminal
    : public introterminal_private::impl_base<
          I, Tags, IntroterminalMask >
{
    typedef introterminal_private::impl_base<
        I, Tags, IntroterminalMask > impl_base_;
public:

    SAKE_MEMBERWISE_MEM_FUN(
        typename introterminal,
        ( default_constructor )( swap ),
        (( impl_base_ ))
    )
    SAKE_MEMBERWISE_COPY_TAGS( (( impl_base_ )) )
    SAKE_MEMBERWISE_DESTRUCTOR_TAGS( (( impl_base_ )) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introterminal(R&& r, sake::begin_tag)
        : impl_base_(r, sake::_begin)
    { }
    template< class R >
    introterminal(R&& r, sake::end_tag)
        : impl_base_(r, sake::_end)
    { }
    template< class R, class J >
    introterminal(R&& r, J const & j)
        : impl_base_(r,j)
    { }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    introterminal(R& r, sake::begin_tag)
        : impl_base_(SAKE_AS_LVALUE(r), sake::_begin)
    { }
    template< class R >
    introterminal(R& r, sake::end_tag)
        : impl_base_(SAKE_AS_LVALUE(r), sake::_end)
    { }
    template< class R, class J >
    introterminal(R& r, J const & j)
        : impl_base_(SAKE_AS_LVALUE(r), j)
    { }

    template< class R >
    introterminal(R const & r, sake::begin_tag)
        : impl_base_(r, sake::_begin)
    { }
    template< class R >
    introterminal(R const & r, sake::end_tag)
        : impl_base_(r, sake::_end)
    { }
    template< class R, class J >
    introterminal(R const & r, J const & j)
        : impl_base_(r,j)
    { }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class J >
    struct implicit_constructor_enabler
        : introterminal_private::implicit_constructor_enabler< introterminal, J >
    { };
public:
    template< class J >
    introterminal(J const & j,
        typename implicit_constructor_enabler<J>::type* = 0)
        : impl_base_(j)
    { }

    template< class J, class Begin, class End >
    introterminal(J const & j, Begin const & begin_, End const & end_)
        : impl_base_(j, begin_, end_)
    { }

private:
    template< class J >
    struct assign_enabler
        : introterminal_private::assign_enabler< introterminal, J >
    { };
public:
    template< class J >
    typename assign_enabler<J>::type
    operator=(J const & j)
    { return impl_base_::operator=(j).derived(); }

private:
    using impl_base_::protected_base;

    friend class sake::iterator::core_access;

    template< class Introterminal >
    struct derived_relax
    {
        typedef typename sake::iterator::adapt_introterminal<
            I, Introterminal, IntroterminalMask >::type type;
    };

    friend class sake::conversion_operators::core_access;

    template< class T >
    T derived_convert(sake::type_tag<T>) const
    { return impl_base_::base(); }
};

} // namespace adaptors

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_ADAPTORS_INTROTERMINAL_HPP
