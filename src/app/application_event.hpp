#pragma once

#include "basic_event.hpp"

namespace indigo
{
class event_visitor;

class app_terminate_event : public basic_event
{
public:
	bool accept(event_visitor& v) const override;
};
}
