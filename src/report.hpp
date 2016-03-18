#ifndef _PARSICAL_REPORT_HPP_
#define _PARSICAL_REPORT_HPP_

#include <string>

namespace parsical {

namespace parser {
struct Location;
}

class Report
{
    Report() = default;
    ~Report() = default;
public:
    void error(std::string msg) const;
    void warn(std::string msg) const;

    static std::string positionedMessage(
            parser::Location &loc,
            std::string msg,
            std::string info
        );

    static std::string positionedMessage(
            parser::Location &loc,
            std::string msg
        );

    static std::string locationToString(parser::Location &loc);

    static Report &getReport();
};

}

#endif
