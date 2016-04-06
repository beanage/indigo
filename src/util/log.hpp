#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace indigo {
static const std::string default_log = "default.log";

void log(const std::string& msg)
{
    static std::ofstream log_file;
    if (!log_file.is_open())
        log_file.open(default_log);

    log_file << msg << std::endl;
    std::cout << msg << std::endl;
}
}
