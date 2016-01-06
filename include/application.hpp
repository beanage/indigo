#ifndef __APPLICATION_HPP_INCLUDED__
#define __APPLICATION_HPP_INCLUDED__

#include <chrono>
#include <ratio>

namespace indigo
{
class application
{
public:
    static const std::chrono::milliseconds step_time;

	application();
	virtual ~application();

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void render(float time_factor) = 0;

    virtual void run(int argc, char const** argv);

    void quit();

private:
	application(const application&) = delete;

    bool quit_;
};
}

#endif
