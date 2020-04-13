#include <systems/move_system.h>

using namespace Halley;

class MoveSystem final : public MoveSystemBase<MoveSystem> {
public:
	void update(Time time)
    {
		for (auto& e: mainFamily) {
			if (e.moveAnimation.moving) {
				e.moveAnimation.curTime += float(time);
				float t = clamp(e.moveAnimation.curTime / e.moveAnimation.len, 0.0f, 1.0f);
				if (t > 0.9999f) {
					t = 1.0f;
					e.moveAnimation.moving = false;
				}

				const auto start = e.moveAnimation.start;
				const auto end = e.moveAnimation.end;
				const auto type = e.moveAnimation.moveType;
				e.transform2D.setGlobalPosition(getPosition(start, end, t, type));
			}
		}
	}

	void onMessageReceived(const MoveMessage& msg, MainFamily& e)
	{
		e.moveAnimation.start = e.transform2D.getGlobalPosition();
		e.moveAnimation.end = msg.to;
		e.moveAnimation.curTime = 0;
		e.moveAnimation.len = msg.len;
		e.moveAnimation.moving = true;
		e.moveAnimation.moveType = msg.moveType;
	}

private:
	Vector2f getPosition(Vector2f start, Vector2f end, float t, MoveType moveType)
	{
		switch (moveType) {
		case MoveType::Conveyour:
			return lerp(start, end, smoothCos(t));

		case MoveType::Jump:
			{
				float parabola = 4 * t * (1 - t);
				return lerp(start, end, t) + Vector2f(0, parabola * -20.0f);
			}

		default:
			return end;
		}
	}
};

REGISTER_SYSTEM(MoveSystem)

