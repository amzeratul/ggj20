#include "game_stage.h"
#include "registry.h"
#include "components/sprite_component.h"
#include "components/position_component.h"
#include "components/camera_component.h"
#include "components/rhythm_area_component.h"
#include "sprite_layers.h"

void GameStage::init()
{
	painterService = std::make_unique<PainterService>();
	inputService = std::make_unique<InputService>();

	world = createWorld("stages/game_stage", createSystem, createComponent);
	world->addService(painterService);
	world->addService(inputService);

	world->createEntity()
		.addComponent(SpriteComponent(Sprite().setImage(getResources(), "background.png"), int(SpriteLayers::Background), 1))
		.addComponent(PositionComponent(Vector2f(0.0f, 0.0f)));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(192.0f, 108.0f)))
		.addComponent(CameraComponent(3.0f, Colour4f(0.0f, 0.0f, 0.0f), 1, 0));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(133, 108)))
		.addComponent(RhythmAreaComponent(2, -1.5f, 0.5f));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(192, 50)))
		.addComponent(RhythmAreaComponent(3, -1.0f, 0.5f));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(251, 108)))
		.addComponent(RhythmAreaComponent(1, -0.5f, 0.5f));

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(192, 166)))
		.addComponent(RhythmAreaComponent(0, 0.0f, 0.5f));
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
