#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace indigo
{
namespace log
{
static const std::string default_log = "default.log";

template <typename T>
void write_impl(T&& v)
{
    static std::ofstream log_file;
    if (!log_file.is_open())
        log_file.open(default_log);

    log_file << v;
    std::cout << v;
}

template <typename T, typename ...Tail>
void write_impl(T&& v, Tail&&... tail)
{
	write_impl(v);
	write_impl(tail...);
}

template <typename ...Args>
void write(Args&&... args)
{
	write_impl(args..., "\n");
}
}
}
