#include "game_stage.h"
#include "registry.h"

void GameStage::init()
{
	painter = std::make_unique<PainterService>();

	world = createWorld("game_world", createSystem);
}

void GameStage::onVariableUpdate(Time t)
{
	world->step(TimeLine::VariableUpdate, t);
}

void GameStage::onRender(RenderContext& rc) const
{
	world->render(rc);
}
