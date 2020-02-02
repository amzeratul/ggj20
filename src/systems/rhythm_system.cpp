#include <systems/rhythm_system.h>

using namespace Halley;

class RhythmSystem final : public RhythmSystemBase<RhythmSystem> {
public:
	void update(Time t)
    {
		updateInput(getInputService().getInput());

		getRhythmService().update(t);

		const int curBeat = getRhythmService().getCurrentBeat();
		const float curBeatTime = getRhythmService().getBeatTime(curBeat);
		const float beatLength = getRhythmService().getBeatLength();

		if (lastBeat != curBeat) {
			lastBeat = curBeat;
			onNewBeat(curBeat);
		}
		if (lastHalfBeat != curBeat && getRhythmService().getCurrentTime() > curBeatTime + beatLength * 0.55f) {
			lastHalfBeat = curBeat;
			onNewHalfBeat(curBeat);
		}
		
		for (auto& e: mainFamily) {
			e.rhythmArea.elapsed += float(t);

			float t = e.rhythmArea.elapsed / e.rhythmArea.totalTime;
			if (t > 1.25f) {
				getWorld().destroyEntity(e.entityId);
			}
		}

		float bounceTime = (getRhythmService().getCurrentTime() - curBeatTime) / beatLength;
		for (auto& e: bouncyFamily) {
			e.bouncy.bounceTime = bounceTime;
		}
	}

private:
	int lastBeat = -1;
	int lastHalfBeat = -1;

	void createBeatMarker(int id, BlacksmithActions action)
	{
		const int curItemLevel = getItemService().getItemLevel(getRhythmService().getCurItemId());
		const bool fromMemory = curItemLevel == 3;
		
		const auto effectiveAction = fromMemory ? BlacksmithActions::Idle : action;
		Vector2f pos = BlacksmithActionsUtils::actionToPos(effectiveAction);
		
		const float beatLen = getRhythmService().getBeatLength();
		getWorld().createEntity()
			.addComponent(PositionComponent(pos))
			.addComponent(RhythmAreaComponent(id, -0.5f * beatLen, 1.5f * beatLen, effectiveAction));

		if (fromMemory) {
			getUIService().showMessage("From memory!", 2 * beatLen + 0.05f);
		}
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

		int timeToNext = getRhythmService().getTimeToNextAction(curBeat);
		if (timeToNext > 0) {
			getUIService().showMessage(toString(timeToNext), getRhythmService().getBeatLength() + 0.05f);
		}
	}

	void onNewHalfBeat(int curBeat)
	{
		// Missed beat?
		if (getRhythmService().hasMissedBeat(curBeat)) {
			onIncorrectInput(curBeat);
			getRhythmService().onBeatMiss(curBeat);
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
					onIncorrectInput(closest);
				}
			} else {
				// Hit out of time
				getRhythmService().onBeatMiss(closest);
				onIncorrectInput(closest);
			}
		}
	}

	void onIncorrectInput(int beat)
	{
		// TODO: sound/visual feedback
		getItemService().onMiss(beat);
		getUIService().showMessage("Missed!", 1.0f);
	}
};

REGISTER_SYSTEM(RhythmSystem)

