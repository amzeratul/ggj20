#include "rhythm_service.h"

void RhythmService::start(AudioAPI& api)
{
	music = api.playMusic("music0");
	setCurrentTime(0);
}

void RhythmService::update(Time t)
{
	setCurrentTime(float(currentTime + t));
	// TODO: sync with music
}

void RhythmService::setCurrentTime(float time)
{
	currentTime = time;
	currentBeat = int(floor(time * bpm / 60.0f));
}

float RhythmService::getCurrentTime() const
{
	return currentTime;
}

int RhythmService::getCurrentBeat() const
{
	return currentBeat;;
}

float RhythmService::getBeatTime(int beat) const
{
	return float(beat) * 60.0f / bpm;
}

void RhythmService::queueActions(std::vector<BlacksmithActions> actions, int firstBeat)
{
	actionsQueued.resize(firstBeat + actions.size(), BlacksmithActions::Idle);
	for (int i = firstBeat; i < int(actions.size()); ++i) {
		actionsQueued[i + firstBeat] = actions[i];
	}
}

BlacksmithActions RhythmService::getActionAtBeat(int beat) const
{
	if (beat < int(actionsQueued.size())) {
		return actionsQueued.at(beat);
	} else {
		return BlacksmithActions::Idle;
	}
}
