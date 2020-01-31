#include "game_stage.h"
#include "registry.h"
#include "components/sprite_component.h"
#include "components/position_component.h"
#include "components/camera_component.h"

void GameStage::init()
{
	painterService = std::make_unique<PainterService>();
	inputService = std::make_unique<InputService>();

	world = createWorld("stages/game_stage", createSystem, createComponent);
	world->addService(painterService);
	world->addService(inputService);

	world->createEntity()
		.addComponent(SpriteComponent(Sprite().setImage(getResources(), "dummy.png"), 0, 1))
		.addComponent(PositionComponent(Vector2f(160.0f, 90.0f)));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(160.0f, 90.0f)))
		.addComponent(CameraComponent(6.0f, Colour4f(0.0f, 0.0f, 0.0f), 1, 0));
}

void GameStage::onVariableUpdate(Time t)
{
	world->step(TimeLine::VariableUpdate, t);
}

void GameStage::onRender(RenderContext& rc) const
{
	painterService->spritePainter.start(300);
	world->render(rc);
}
