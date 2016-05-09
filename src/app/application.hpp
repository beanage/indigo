#pragma once

#include <chrono>
#include <memory>
#include <vector>
#include <ratio>
#include <platform/bitmap.hpp>

namespace indigo
{
class basic_event;
class basic_event_factory;
class event_visitor;

template<class T> class resource_manager;
class model;
class shader;
class bitmap;
class mesh;
class renderer;

class application
{
    friend class __run;

public:
    static const unsigned int gl_major_version;
    static const unsigned int gl_minor_version;

    static const std::chrono::milliseconds update_intervall;

    application();
    virtual ~application();

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render(renderer&) = 0;

    virtual void event(const basic_event& e);

    virtual void pathes(resource_manager<model>& mgr) {}
    virtual void pathes(resource_manager<mesh>& mgr) {}
    virtual void pathes(resource_manager<shader>& mgr) {}
    virtual void pathes(resource_manager<bitmap>& mgr) {}

    void terminate();
    bool terminated() const;

protected:
    virtual void add_event_handler(event_visitor* h);
    virtual void remove_event_handler(event_visitor* h);

private:
    application(const application&) = delete;
    void setup_resource_managers();

    void poll_events();

    bool quit_;
    float fps_;
    float ups_;
    std::vector<event_visitor*> event_handler_;
};



void init_gl();
class __run {
    template<class T>
    friend void run(int argc, char const** argv);
private:
    static void internal_run(application &app, int argc, char const **argv);
};

template<class app>
void run(int argc, char const** argv) {
    init_gl();
    app a;
    __run::internal_run(a, argc, argv);
}
}
