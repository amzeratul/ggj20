#include <systems/rhythm_system.h>

using namespace Halley;

class RhythmSystem final : public RhythmSystemBase<RhythmSystem> {
public:
	void update(Time t)
    {
		updateInput(getInputService().getInput());

		getRhythmService().update(t);

		if (lastBeat != getRhythmService().getCurrentBeat()) {
			lastBeat = getRhythmService().getCurrentBeat();
			onNewBeat(lastBeat);
		}
		
		for (auto& e: mainFamily) {
			e.rhythmArea.elapsed += float(t);

			float t = e.rhythmArea.elapsed / e.rhythmArea.totalTime;
			if (t > 1.25f) {
				getWorld().destroyEntity(e.entityId);
			}
		}
	}

private:
	int lastBeat = -1;

	void createBeatMarker(int id, BlacksmithActions action)
	{
		Vector2f pos = BlacksmithActionsUtils::actionToPos(action);
		
		const float beatLen = getRhythmService().getBeatLength();
		getWorld().createEntity()
			.addComponent(PositionComponent(pos))
			.addComponent(RhythmAreaComponent(id, -0.5f * beatLen, 1.5f * beatLen));
	}

	void onNewBeat(int curBeat)
	{
		// Draw next beat
		int lookAhead = 2;
		//Logger::logInfo("On beat " + toString(curBeat));
		auto action = getRhythmService().getActionAtBeat(curBeat + lookAhead);

		if (action != BlacksmithActions::Idle) {
			createBeatMarker(curBeat + lookAhead, action);
		}

		// Missed last beat?
		if (getRhythmService().hasMissedBeat(curBeat - 1)) {
			onIncorrectInput();
		}
	}

	void updateInput(InputVirtual& input)
	{
		if (input.isAnyButtonPressed()) {
			const int closest = getRhythmService().getClosestBeat();
			auto curTime = getRhythmService().getCurrentTime();
			auto beatTime = getRhythmService().getBeatTime(closest);

			// Distance is 0 at the exact time, -1/1 at completely off
			float distance = 2 * (curTime - beatTime) / getRhythmService().getBeatLength();
			const float threshold = 0.5f;
			if (std::abs(distance) < threshold) {
				// Hit on time
				BlacksmithActions type = BlacksmithActions::Idle;
				if (input.isButtonPressed(0)) {
					type = BlacksmithActions::Anvil;
				} else if (input.isButtonPressed(1)) {
					type = BlacksmithActions::Love;
				} else if (input.isButtonPressed(2)) {
					type = BlacksmithActions::Bucket;
				} else if (input.isButtonPressed(3)) {
					type = BlacksmithActions::Furnace;
				}
				
				bool correct = getRhythmService().onBeatInput(closest, type);
				if (!correct) {
					onIncorrectInput();
				}
			} else {
				// Hit out of time
				getRhythmService().onBeatMiss(closest);
				onIncorrectInput();
			}
		}
	}

	void onIncorrectInput()
	{
		// TODO: sound/visual feedback
		getItemService().onMiss();
	}
};

REGISTER_SYSTEM(RhythmSystem)

