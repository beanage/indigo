#ifndef __APPLICATION_HPP_INCLUDED__
#define __APPLICATION_HPP_INCLUDED__

#include <chrono>
#include <ratio>

namespace indigo {
class application
{
    friend void run(application&, int, char const**);

public:
    static const unsigned int gl_major_version;
    static const unsigned int gl_minor_version;

    static const std::chrono::milliseconds update_intervall;

    application();
    virtual ~application();

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render(float time_factor) = 0;

    void terminate();
    bool terminated() const;

private:
    application(const application&) = delete;

    bool quit_;
    float fps_;
    float ups_;
};

void init_gl();
void run(application& app, int argc, char const** argv);
}

#endif
