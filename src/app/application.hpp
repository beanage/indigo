#ifndef __APPLICATION_HPP_INCLUDED__
#define __APPLICATION_HPP_INCLUDED__

#include <chrono>
#include <memory>
#include <vector>
#include <ratio>

namespace indigo
{
class basic_event;
class basic_event_factory;
class event_visitor;

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

    virtual void event(const basic_event& e);

    void terminate();
    bool terminated() const;

protected:
    virtual void add_event_handler(event_visitor* h);
    virtual void remove_event_handler(event_visitor* h);

private:
    application(const application&) = delete;

    void poll_events();

    bool quit_;
    float fps_;
    float ups_;
    std::vector<event_visitor*> event_handler_;
};

void init_gl();
void run(application& app, int argc, char const** argv);
}

#endif
