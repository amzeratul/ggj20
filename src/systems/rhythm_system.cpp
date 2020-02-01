#include <systems/rhythm_system.h>

using namespace Halley;

class RhythmSystem final : public RhythmSystemBase<RhythmSystem> {
public:
	void init()
	{
		auto& world = getWorld();
		world.createEntity()
			.addComponent(PositionComponent(Vector2f(133, 108)))
			.addComponent(RhythmAreaComponent(2, -1.5f, 0.5f));

		world.createEntity()
			.addComponent(PositionComponent(Vector2f(192, 50)))
			.addComponent(RhythmAreaComponent(3, -1.0f, 0.5f));

		world.createEntity()
			.addComponent(PositionComponent(Vector2f(251, 108)))
			.addComponent(RhythmAreaComponent(1, -0.5f, 0.5f));

		world.createEntity()
			.addComponent(PositionComponent(Vector2f(192, 166)))
			.addComponent(RhythmAreaComponent(0, 0.0f, 0.5f));

		getRhythmService().start(*getAPI().audio);
	}
	
	void update(Time t)
    {
		getRhythmService().update(t);
		for (auto& e: mainFamily) {
			e.rhythmArea.elapsed += float(t);
		}
	}
};

REGISTER_SYSTEM(RhythmSystem)

