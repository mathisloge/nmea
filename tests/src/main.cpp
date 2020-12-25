#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <nmea/nmea_parser.hpp>
class Handler
{
public:
    std::string got_msg;
    Handler() : got_msg{""} {}
    void handle(const NmeaMessage &msg) { got_msg.append("nmea"); }
    void handle(const NmeaUnsupported &msg) { got_msg.append("unsupported"); }
    void handle(const NmeaDTM &msg) { got_msg.append("dtm"); }
    void handle(const NmeaGLL &msg) { got_msg.append("gll"); }
};

TEST_CASE("simple single messages", "[single]")
{
    Handler handler;
    SECTION("DTM")
    {
        std::string data = "$GPDTM,999,,0.08,N,0.07,E,-47.7,W84*1B\r\n";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.end());
        REQUIRE(handler.got_msg == "dtm");
    }
    SECTION("GLL")
    {
        std::string data = "$GNGLL,5304.98825,N,00852.25036,E,221313.00,A,A*76\r\n";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.end());
        REQUIRE(handler.got_msg == "gll");
    }
    SECTION("UNSUPPORTED")
    {
        std::string data = "$GLGSV,3,2,09,79,17,167,,80,65,215,,81,33,049,,82,82,345,,3*71\r\n";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.end());
        REQUIRE(handler.got_msg == "unsupported");
    }
    SECTION("not valid - missing end")
    {
        std::string data = "$GLGSV,3,2,09,79,17,167,,80,65,215,,81,33,049,,82,82,345,,3*71";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.begin());
        REQUIRE(handler.got_msg.empty());
    }
    SECTION("not valid - missing start")
    {
        std::string data = "GLGSV,3,2,09,79,17,167,,80,65,215,,81,33,049,,82,82,345,,3*71\r\n";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.begin());
        REQUIRE(handler.got_msg.empty());
    }
    SECTION("not valid - false crc")
    {
        // consume data but do not dispatch
        std::string data = "$GNGLL,5304.98825,N,00852.25036,E,221313.00,A,A*00\r\n";
        auto it = data.begin();
        nmea::parseOneWithDispatch(it, data.end(), handler);
        REQUIRE(it == data.end());
        REQUIRE(handler.got_msg == "");
    }
}