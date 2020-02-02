#include "game_stage.h"
#include "registry.h"
#include "components/sprite_component.h"
#include "components/position_component.h"
#include "components/camera_component.h"
#include "sprite_layers.h"
#include "services/rhythm_service.h"
#include "services/item_service.h"
#include "services/ui_service.h"
#include "game.h"

void GameStage::init()
{
	auto& game = dynamic_cast<GGJ20Game&>(getGame());
	
	painterService = std::make_unique<PainterService>();
	rhythmService = std::make_unique<RhythmService>();
	itemService = std::make_unique<ItemService>();

	itemService->setItems(ItemCollection(getResources().get<ConfigFile>("gameplay/items")->getRoot()));

	world = createWorld("stages/game_stage", createSystem, createComponent);
	world->addService(painterService);
	world->addService(game.getInputService());
	world->addService(rhythmService);
	world->addService(itemService);
	world->addService(std::make_shared<UIService>());

	world->createEntity()
		.addComponent(PositionComponent(Vector2f(192.0f, 108.0f)))
		.addComponent(CameraComponent(game.getZoom(), Colour4f(0.0f, 0.0f, 0.0f), 1, 0));
}

void GameStage::onVariableUpdate(Time t)
{
	dynamic_cast<GGJ20Game&>(getGame()).getInputService()->update(t);
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
