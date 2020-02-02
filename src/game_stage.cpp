#include "game_stage.h"
#include "registry.h"
#include "components/sprite_component.h"
#include "components/position_component.h"
#include "components/camera_component.h"
#include "sprite_layers.h"
#include "services/rhythm_service.h"
#include "services/item_service.h"
#include "services/ui_service.h"

void GameStage::init()
{
	painterService = std::make_unique<PainterService>();
	inputService = std::make_unique<InputService>(getInputAPI());
	rhythmService = std::make_unique<RhythmService>();
	itemService = std::make_unique<ItemService>();

	itemService->setItems(ItemCollection(getResources().get<ConfigFile>("gameplay/items")->getRoot()));

	world = createWorld("stages/game_stage", createSystem, createComponent);
	world->addService(painterService);
	world->addService(inputService);
	world->addService(rhythmService);
	world->addService(itemService);
	world->addService(std::make_shared<UIService>());

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(192.0f, 108.0f)))
		.addComponent(CameraComponent(3.0f, Colour4f(0.0f, 0.0f, 0.0f), 1, 0));
}

void GameStage::onVariableUpdate(Time t)
{
	inputService->update(t);
	world->step(TimeLine::VariableUpdate, t);

	if (itemService->needsRestart()) {
		getAPI().core->setStage(std::make_unique<GameStage>());
	}
}

void GameStage::onRender(RenderContext& rc) const
{
	painterService->spritePainter.start(300);
	world->render(rc);
}
