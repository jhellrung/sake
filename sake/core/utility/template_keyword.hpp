/*******************************************************************************
 * core/utility/template_keyword.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_TEMPLATE_KEYWORD( Keyword )
 * SAKE_TEMPLATE_KEYWORD_DEFAULT( Keyword, Default )
 *
 * struct insert_keyword_value_if_c< Cond, Sequence, Keyword, Value >
 * struct insert_keyword_value_if< Cond, Sequence, Keyword, Value >
 * struct lazy_insert_keyword_value_if_c< Cond, Sequence, Keyword, Value >
 * struct lazy_insert_keyword_value_if< Cond, Sequence, Keyword, Value >
 *
 * This header provides a framework to define and use a keyword set to specify
 * template parameters.
 *
 * Example:
 *
 * namespace X_keyword
 * {
 * SAKE_TEMPLATE_KEYWORD( x )
 * SAKE_TEMPLATE_KEYWORD( y )
 * typedef ... default_x_type;
 * typedef ... default_y_type;
 * } // namespace X_keyword
 *
 * template< class TemplateParameters >
 * struct X
 * {
 *     BOOST_MPL_ASSERT((boost::mpl::is_sequence< TemplateParameters >));
 *     typedef typename boost_ext::mpl::at<
 *         TemplateParameters,
 *         X_keyword::tag::x,
 *         X_keyword::default_x_type
 *     >::type x_type;
 *     typedef typename boost_ext::mpl::at<
 *         TemplateParameters,
 *         X_keyword::tag::y,
 *         X_keyword::default_y_type
 *     >::type y_type;
 * };
 *
 * using boost::mpl::map;
 * using X_keyword::x;
 * using X_keyword::y;
 * X< map<> >
 * X< map< x< int > > > // x_type == int
 * X< map< y< int > > > // y_type == int
 * X< map< x< float >, y< double > > > // x_type == float, y_type == double
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_TEMPLATE_KEYWORD_HPP
#define SAKE_CORE_UTILITY_TEMPLATE_KEYWORD_HPP

#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>

#define SAKE_TEMPLATE_KEYWORD( Keyword ) \
namespace tag { struct Keyword; } \
template< class Value > struct Keyword \
    : ::boost::mpl::pair< tag::Keyword, Value > \
{ };

#define SAKE_TEMPLATE_KEYWORD_DEFAULT( Keyword, Default ) \
namespace tag { struct Keyword; } \
template< class Value = Default > struct Keyword \
    : ::boost::mpl::pair< tag::Keyword, Value >\
{ };

namespace sake
{

template<
    bool Cond,
    class Sequence,
    template<class> class Keyword, class Value
>
struct insert_keyword_value_if_c;

template<
    class Sequence,
    template<class> class Keyword, class Value
>
struct insert_keyword_value_if_c< false, Sequence, Keyword, Value >
{ typedef Sequence type; };

template<
    class Sequence,
    template<class> class Keyword, class Value
>
struct insert_keyword_value_if_c< true, Sequence, Keyword, Value >
    : boost::mpl::insert< Sequence, Keyword< Value > >
{ };

template<
    class Cond,
    class Sequence,
    template<class> class Keyword, class Value
>
struct insert_keyword_value_if
    : insert_keyword_value_if_c< Cond::value, Sequence, Keyword, Value >
{ };

template<
    bool Cond,
    class Sequence,
    template<class> class Keyword, class Value
>
struct lazy_insert_keyword_value_if_c;

template<
    class Sequence,
    template<class> class Keyword, class Value
>
struct lazy_insert_keyword_value_if_c< false, Sequence, Keyword, Value >
{ typedef Sequence type; };

template<
    class Sequence,
    template<class> class Keyword, class Value
>
struct lazy_insert_keyword_value_if_c< true, Sequence, Keyword, Value >
    : boost::mpl::insert< Sequence, Keyword< typename Value::type > >
{ };

template<
    class Cond,
    class Sequence,
    template<class> class Keyword, class Value
>
struct lazy_insert_keyword_value_if
    : lazy_insert_keyword_value_if_c< Cond::value, Sequence, Keyword, Value >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_TEMPLATE_KEYWORD_HPP
