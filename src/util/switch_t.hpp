#pragma once

namespace indigo {
// usage:
//     std::string input = ... ;
//
//     switch_t(input)
//         .case_t("A", [&](){
//             printf("A selected\n");
//         })
//         .case_t("B", ...
//             ...
//         )
//         .default_t(...
//             ...
//         );
//
template <typename T>
struct switch_t
{
    switch_t(T const& v)
        : v_(v), break_(false)
    {
    }

    template <typename F>
    switch_t<T>& case_t(T const& c, F f)
    {
        if (!break_ && v_ == c) {
            f();
            break_ = true;
        }

        return *this;
    }

    template <typename F>
    void default_t(F f)
    {
        if (!break_)
            f();
    }

private:
    T const& v_;
    bool break_;
};
}
