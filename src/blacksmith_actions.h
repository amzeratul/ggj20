#pragma once

#include <halley.hpp>
using namespace Halley;

enum class BlacksmithActions {
    Idle = -1,
    Anvil = 0,
    Love = 1,
    Furnace = 2,
    Bucket = 3
};

class BlacksmithActionsUtils {
public:
	static Vector2f actionToPos(BlacksmithActions action) {
		switch (action) {
		case BlacksmithActions::Anvil:
			return Vector2f(192, 166);
		case BlacksmithActions::Bucket:
			return Vector2f(133, 108);
		case BlacksmithActions::Furnace:
			return Vector2f(192, 50);
		case BlacksmithActions::Love:
			return Vector2f(251, 108);
		}
		return Vector2f(192, 108);
	}
};