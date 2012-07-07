/*******************************************************************************
 * sake/core/range/forward_traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_forward_traits<R>
 *
 * struct range_forward_iterator<R>
 * struct range_forward_reference<R>
 * struct range_forward_traversal<R>
 *
 * range_forward_traits has the following interface.
 *
 * template< class R >
 * struct range_forward_traits
 * {
 *     typedef ... value_type;
 *     typedef ... difference_type;
 *     typedef ... size_type;
 *
 *     typedef ... base_iterator;
 *     typedef ... base_reference;
 *     typedef ... base_traversal;
 *
 *     template< class Introterminal >
 *     struct base_iterator_with { typedef ... type; };
 *     template< class Begin, class End >
 *     struct base_subrange_with { typedef ... type; };
 *
 *     static bool
 *     empty(R const & r);
 *     static size_type
 *     size(R const & r);
 *     static difference_type
 *     distance(R const & r);
 *
 *     typedef ... iterator;
 *     typedef ... reference;
 *     typedef ... traversal;
 *
 *     template< class Introterminal >
 *     struct iterator_with { typedef ... type; };
 *     template< class Begin, class End >
 *     struct subrange_with { typedef ... type; };
 *
 *     static iterator
 *     begin(R&& r);
 *     template< class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     begin(R&& r, Introterminal);
 *
 *     static iterator
 *     end(R&& r);
 *     template< class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     end(R&& r, Introterminal);
 *
 *     template< class T, class Introterminal >
 *     static typename iterator_with< Introterminal >::type
 *     iter_at(R&& r, T x, Introterminal);
 *
 *     template< class Begin, class End >
 *     static typename subrange_with< Begin, End >::type
 *     sub(R&& r, Begin b, End e);
 *
 *     template< class I >
 *     static reference
 *     at(R&& r, I i);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_HPP
#define SAKE_CORE_RANGE_FORWARD_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/adaptors/fwd.hpp>
#include <sake/core/iterator/adaptors/move.hpp>
#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/move.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>
#include <sake/core/range/has_value_ownership.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace range_forward_traits_private
{

template< class T >
inline typename T::base_type const &
base_helper(T const & x)
{ return x.base(); }

inline sake::begin_tag
base_helper(sake::begin_tag)
{ return sake::_begin; }

inline sake::end_tag
base_helper(sake::end_tag)
{ return sake::_end; }

template< class R >
struct dispatch_bool
{
    typedef typename sake::range_reference<R>::type reference;
#ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value = boost_ext::mpl::and2<
        sake::range_has_value_ownership<R>,
        boost_ext::is_lvalue_reference_to_nonconst< reference >
    >::value;
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
    static bool const value = boost_ext::mpl::and3<
        sake::range_has_value_ownership<R>,
        boost_ext::is_lvalue_reference_to_nonconst< reference >,
        sake::has_move_emulation<
            typename boost_ext::remove_reference< reference >::type >
    >::value;
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES
};

template< class R, bool = dispatch_bool<R>::value >
struct dispatch;

template< class R >
struct impl
    : dispatch<R>
{ };

template< class R >
struct dispatch< R, false >
    : sake::range_traits<R>
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introterminal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introterminal >
    { };
    template< class Begin, class End >
    struct base_subrange_with
        : base_traits::template subrange_with< Begin, End >
    { };

private:
    typedef typename boost_ext::add_reference<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
public:

    static base_iterator
    begin(param_type r)
    { return base_traits::begin(SAKE_AS_LVALUE(r)); }
    template< class Introterminal >
    static typename base_iterator_with< Introterminal >::type
    begin(param_type r, Introterminal)
    { return base_traits::begin(SAKE_AS_LVALUE(r), Introterminal()); }

    static base_iterator
    end(param_type r)
    { return base_traits::end(SAKE_AS_LVALUE(r)); }
    template< class Introterminal >
    static typename base_iterator_with< Introterminal >::type
    end(param_type r, Introterminal)
    { return base_traits::end(SAKE_AS_LVALUE(r), Introterminal()); }

    template< class T, class Introterminal >
    static typename base_iterator_with< Introterminal >::type
    iter_at(param_type r, T const & x, Introterminal)
    { return base_traits::iter_at(SAKE_AS_LVALUE(r), x, Introterminal()); }

    template< class Begin, class End >
    static typename base_subrange_with< Begin, End >::type
    sub(param_type r, Begin const & b, End const & e)
    { return base_traits::sub(SAKE_AS_LVALUE(r), b, e); }

    template< class T >
    static base_reference
    at(param_type r, T const x)
    { return base_traits::at(SAKE_AS_LVALUE(r), x); }
};

template< class R >
struct dispatch< R, true >
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introterminal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introterminal >
    { };
    template< class Begin, class End >
    struct base_subrange_with
        : base_traits::template subrange_with< Begin, End >
    { };

    typedef sake::iterator::adaptors::move< base_iterator > iterator;
    typedef typename iterator::reference reference;
    typedef typename iterator::iterator_traversal traversal;

    template< class Introterminal >
    struct iterator_with
    { typedef sake::iterator::adaptors::move<
        typename base_iterator_with< Introterminal >::type > type; };
    template< class Begin, class End >
    struct subrange_with
    { typedef sake::range::adaptors::move<
        typename base_subrange_with< Begin, End >::type > type; };

private:
    typedef typename boost_ext::add_reference<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
public:

    static iterator
    begin(param_type r)
    { return iterator(base_traits::begin(SAKE_AS_LVALUE(r))); }
    template< class Introterminal >
    static typename iterator_with< Introterminal >::type
    begin(param_type r, Introterminal)
    {
        return typename iterator_with< Introterminal >::type(
            base_traits::begin(SAKE_AS_LVALUE(r), Introterminal()));
    }

    static iterator
    end(param_type r)
    { return iterator(base_traits::end(SAKE_AS_LVALUE(r))); }
    template< class Introterminal >
    static typename iterator_with< Introterminal >::type
    end(param_type r, Introterminal)
    {
        return typename iterator_with< Introterminal >::type(
            base_traits::end(SAKE_AS_LVALUE(r), Introterminal()));
    }

    template< class T, class Introterminal >
    static typename iterator_with< Introterminal >::type
    iter_at(param_type r, T const & x, Introterminal)
    {
        return typename iterator_with< Introterminal >::type(
            base_traits::iter_at(SAKE_AS_LVALUE(r),
            range_forward_traits_private::base_helper(x),
            Introterminal())
        );
    }

    template< class Begin, class End >
    static typename subrange_with< Begin, End >::type
    sub(param_type r, Begin const & b, End const & e)
    {
        return typename subrange_with< void, void >::type(base_traits::sub(
            SAKE_AS_LVALUE(r),
            range_forward_traits_private::base_helper(b),
            range_forward_traits_private::base_helper(e)
        ));
    }

    template< class T >
    static reference
    at(param_type r, T const x)
    { return sake::move(base_traits::at(SAKE_AS_LVALUE(r), x)); }
};

template< class R >
struct impl< R& >
    : sake::range_traits<R>
{
    typedef sake::range_traits<R> base_traits;
    typedef typename base_traits::iterator base_iterator;
    typedef typename base_traits::reference base_reference;
    typedef typename base_traits::traversal base_traversal;

    template< class Introterminal >
    struct base_iterator_with
        : base_traits::template iterator_with< Introterminal >
    { };
    template< class Begin, class End >
    struct base_subrange_with
        : base_traits::template subrange_with< Begin, End >
    { };
};

} // namespace range_forward_traits_private

template< class R >
struct range_forward_traits
    : range_forward_traits_private::impl<
          typename boost_ext::remove_rvalue_reference<R>::type >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FORWARD_TRAITS_HPP
