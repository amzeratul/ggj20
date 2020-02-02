#include <systems/game_state_system.h>
#include "src/game_state.h"

using namespace Halley;

class GameStateSystem final : public GameStateSystemBase<GameStateSystem> {
public:
	void init()
	{
		getRhythmService().start(Difficulty::Easy, *getAPI().audio);
	}

	void update(Time t)
	{
		stateTime += t;
		
		if (gameState == GameState::Play) {
			if (!getItemService().isAlive()) {
				setState(GameState::Lose);
			}
		}

		if (gameState == GameState::Lose) {
			if (stateTime > 0.5f) {
				if (getInputService().getInput().isAnyButtonPressed()) {
					getItemService().flagRestart();
				}
			}
		}
	}

private:
	GameState gameState = GameState::Play;
	Time stateTime = 0;

	void setState(GameState state)
	{
		gameState = state;
		stateTime = 0;

		if (state == GameState::Lose) {
			onLose();
		}
	}

	void onLose()
	{
		getAPI().audio->stopAllMusic(1.0f);
	}
};

REGISTER_SYSTEM(GameStateSystem)

