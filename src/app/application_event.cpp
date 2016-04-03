#include "application_event.hpp"
#include "event_visitor.hpp"

using namespace indigo;

bool app_terminate_event::accept(event_visitor& v) const
{
	return v.visit(*this);
}
