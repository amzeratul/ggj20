#include "rhythm_service.h"

void RhythmService::start(AudioAPI& api)
{
	music = api.playMusic("music/music0");
	setCurrentTime(0);
}

void RhythmService::update(Time t)
{
	setCurrentTime(float(currentTime + t));
	// TODO: sync with music
}

float RhythmService::getBPM() const
{
	return bpm;
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
	return currentBeat;
}

int RhythmService::getClosestBeat() const
{
	return lround(currentTime * bpm / 60.0f);
}

int RhythmService::getItemStartBeat() const
{
	return itemStartsAt;
}

int RhythmService::getItemEndBeat() const
{
	return itemEndsAt;
}

float RhythmService::getBeatTime(int beat) const
{
	return float(beat) * 60.0f / bpm;
}

void RhythmService::onNewItem(const ItemConfig& item)
{
	int nextBeat = getCurrentBeat() + 4;
	int alignedBeat = alignUp(nextBeat, 4);
	if (nextBeat != alignedBeat) {
		Logger::logWarning("nextBeat and alignedBeat don't match!");
	} else {
		//Logger::logInfo("Queued " + item.id + " at " + toString(alignedBeat));
	}
	queueActions(item.actions, alignedBeat);
	itemStartsAt = alignedBeat;
	itemEndsAt = alignedBeat + int(item.actions.size());
	currentItemOK = true;
}

bool RhythmService::isItemOK() const
{
	if (!currentItemOK) {
		return false;
	}

	// Check if any inputs got missed
	for (int i = itemStartsAt; i < itemEndsAt; ++i) {
		if (playerInputRegistered[i] == 0 && actionsQueued[i] != BlacksmithActions::Idle) {
			return false;
		}
	}

	return true;
}

void RhythmService::queueActions(std::vector<BlacksmithActions> actions, int firstBeat)
{
	actionsQueued.resize(firstBeat + actions.size(), BlacksmithActions::Idle);
	playerInputRegistered.resize(actionsQueued.size(), 0);
	for (int i = 0; i < int(actions.size()); ++i) {
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

bool RhythmService::onBeatInput(int beat, BlacksmithActions action)
{
	if (isValidBeatInput(beat, action)) {
		return true;
	} else {
		currentItemOK = false;
		return false;
	}
}

bool RhythmService::isValidBeatInput(int beat, BlacksmithActions action)
{
	if (beat >= playerInputRegistered.size() || playerInputRegistered.at(beat) != 0) {
		// Already registered or not expecting input here
		return false;
	}
	playerInputRegistered[beat] = 1;
	return getActionAtBeat(beat) == action;
}

float RhythmService::getBeatLength() const
{
	return 60.0f / bpm;
}
