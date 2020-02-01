#pragma once

#include <halley.hpp>
using namespace Halley;

class InputService : public Service {
public:
	InputService(InputAPI& api);

	void update(Time t);

	InputVirtual& getInput();

private:
	std::shared_ptr<InputVirtual> device;
};
