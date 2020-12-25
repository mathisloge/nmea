#pragma once
#include <vector>
#include "nmea_parser_helper.hpp"

namespace nmea
{
    template <typename Iterator, typename Handler>
    bool parseOneWithDispatch(Iterator &begin, Iterator end, Handler &handler)
    {
        detail::AllNmeaMessages msg_var;
        Iterator begin_copy = begin;
        while (begin_copy != end && (*begin_copy) != '$')
            begin_copy++;
        if (begin_copy == end)
            return false;

        const auto success = detail::parse_nmea(begin_copy, end, msg_var);
        if (success == detail::ParseResult::ok)
        {
            begin = begin_copy;
            boost::apply_visitor([&handler](const auto &msg) { handler.handle(msg); }, msg_var);
            return true;
        }
        // we have detected a complete message but the crc is invalid. Move the iterator to the end of the message but do not dispatch
        if (success == detail::ParseResult::crc_invalid)
            begin = begin_copy;
        return false;
    }
} // namespace nmea
