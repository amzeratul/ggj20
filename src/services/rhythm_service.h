#pragma once

#include <halley.hpp>
#include "src/blacksmith_actions.h"
using namespace Halley;

class RhythmService : public Service {
public:
	void start(AudioAPI& api);
	void update(Time t);
	
	void setCurrentTime(float time);
	float getCurrentTime() const;
	
	int getCurrentBeat() const;
	float getBeatTime(int beat) const;

	void queueActions(std::vector<BlacksmithActions> actions, int firstBeat);
	BlacksmithActions getActionAtBeat(int beat) const;

private:
	float bpm = 120;
	float currentTime = 0;
	int currentBeat = 0;

	std::vector<BlacksmithActions> actionsQueued;
	
	AudioHandle music;
};
