#include <systems/game_state_system.h>
#include "src/game_state.h"
#include "src/game_stage.h"

using namespace Halley;

class GameStateSystem final : public GameStateSystemBase<GameStateSystem> {
public:
	void init()
	{
		getItemService().startStage();
		setDifficulty(getItemService().getDifficulty());
		getAPI().audio->playMusic("sfx/ambience", 1);
	}

	void update(Time t)
	{
		stateTime += t;
		
		if (gameState == GameState::Play) {
			if (getItemService().isStageDone()) {
				setState(GameState::Done);
			} else if (!getItemService().isAlive()) {
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

		if (gameState == GameState::Done) {
			if (stateTime > 1.0f) {
				nextLevel();
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

	void setDifficulty(Difficulty difficulty)
	{
		getRhythmService().start(difficulty, *getAPI().audio);
		String msg;
		switch (difficulty) {
		case Difficulty::Easy:
			msg = "Stage 1";
			break;
		case Difficulty::Normal:
			msg = "Stage 2";
			break;
		case Difficulty::Hard:
			msg = "Stage 3";
			break;
		case Difficulty::SuddenDeath:
			msg = "Sudden Death!";
			break;
		}
		getUIService().showImportantMessage(msg, 4.0f);
	}

	void nextLevel()
	{
		auto nextDifficulty = getItemService().getDifficulty();
		if (nextDifficulty != Difficulty::SuddenDeath) {
			nextDifficulty = Difficulty(int(nextDifficulty) + 1);
		}
		getItemService().setDifficulty(nextDifficulty);
		getAPI().core->setStage(std::make_unique<GameStage>(getItemService().getSelf()));
	}
};

REGISTER_SYSTEM(GameStateSystem)

