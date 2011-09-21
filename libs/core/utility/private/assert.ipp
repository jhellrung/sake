/*******************************************************************************
 * libs/core/utility/private/assert.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

void
failure_action::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number) const
{
    functional::print()(o,
        macro, expression, filename, function, line_number
    );
    failure_action_abort();
}

void
failure_action::
operator()(
    std::ostream& o,
    char const * const macro, char const * const expression,
    char const * const filename, char const * const function, unsigned int const line_number,
    unsigned int const subexpression_index, char const * const subexpression) const
{
    functional::print()(o,
        macro, expression, filename, function, line_number,
        subexpression_index, subexpression
    );
    failure_action_abort();
}

#undef failure_action
#undef failure_action_abort
