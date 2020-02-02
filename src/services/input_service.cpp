#include "input_service.h"

InputService::InputService(InputAPI& input)
{
	device = std::make_shared<InputVirtual>(12, 2);

	auto key = input.getKeyboard();
	if (key) {
		device->bindButton(0, key, Keys::S);
		device->bindButton(1, key, Keys::D);
		device->bindButton(2, key, Keys::A);
		device->bindButton(3, key, Keys::W);

		device->bindButton(0, key, Keys::Down);
		device->bindButton(1, key, Keys::Right);
		device->bindButton(2, key, Keys::Left);
		device->bindButton(3, key, Keys::Up);

		device->bindButton(4, key, Keys::Enter);
		device->bindButton(4, key, Keys::KP_Enter);
	}
	
	auto joy = input.getJoystick();
	if (joy) {
		device->bindButton(0, joy, joy->getButtonAtPosition(JoystickButtonPosition::FaceBottom));
		device->bindButton(1, joy, joy->getButtonAtPosition(JoystickButtonPosition::FaceRight));
		device->bindButton(2, joy, joy->getButtonAtPosition(JoystickButtonPosition::FaceLeft));
		device->bindButton(3, joy, joy->getButtonAtPosition(JoystickButtonPosition::FaceTop));

		device->bindButton(4, joy, joy->getButtonAtPosition(JoystickButtonPosition::Start));
	}
}

void InputService::update(Time t)
{
	device->update(t);
}

InputVirtual& InputService::getInput()
{
	return *device;
}
