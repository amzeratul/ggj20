#pragma once

#include <halley.hpp>
#include "src/blacksmith_actions.h"
#include "src/item_config.h"
using namespace Halley;

class RhythmService : public Service {
public:
	void start(AudioAPI& api);
	void update(Time t);
	
	float getBPM() const;
	float getBeatLength() const;

	void setCurrentTime(float time);
	float getCurrentTime() const;

	int getCurrentBeat() const;
	int getClosestBeat() const;
	int getItemStartBeat() const;
	int getItemEndBeat() const;
	float getBeatTime(int beat) const;

	void onNewItem(const ItemConfig& item);
	bool isItemOK() const;

	BlacksmithActions getActionAtBeat(int beat) const;
	bool onBeatInput(int beat, BlacksmithActions action);
	void onBeatMiss(int beat);
	bool hasMissedBeat(int beat);

private:
	float bpm = 120;
	float currentTime = 0;
	int currentBeat = 0;
	int itemStartsAt = 0;
	int itemEndsAt = 0;
	bool currentItemOK = false;

	std::vector<BlacksmithActions> actionsQueued;
	std::vector<char> playerInputRegistered;
	
	AudioHandle music;

	void queueActions(std::vector<BlacksmithActions> actions, int firstBeat);
	bool isValidBeatInput(int beat, BlacksmithActions action);
};
