#pragma once

namespace indigo
{
class app_terminate_event;
class key_down_event;
class key_up_event;
class mouse_move_event;
class mouse_down_event;
class mouse_up_event;
class mouse_wheel_event;

class event_visitor
{
public:
	virtual ~event_visitor() {}

	virtual bool visit(const app_terminate_event&) { return false; }
	virtual bool visit(const key_down_event&) { return false; }
	virtual bool visit(const key_up_event&) { return false; }
	virtual bool visit(const mouse_move_event&) { return false; }
	virtual bool visit(const mouse_down_event&) { return false; }
	virtual bool visit(const mouse_up_event&) { return false; }
	virtual bool visit(const mouse_wheel_event&) { return false; }

protected:
	event_visitor() = default;
};
}
