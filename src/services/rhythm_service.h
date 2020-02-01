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
	int getItemStartBeat() const;
	int getItemEndBeat() const;
	float getBeatTime(int beat) const;

	void onNewItem(const ItemConfig& item);
	void queueActions(std::vector<BlacksmithActions> actions, int firstBeat);
	BlacksmithActions getActionAtBeat(int beat) const;

private:
	float bpm = 120;
	float currentTime = 0;
	int currentBeat = 0;
	int itemStartsAt = 0;
	int itemEndsAt = 0;

	std::vector<BlacksmithActions> actionsQueued;
	
	AudioHandle music;
};
