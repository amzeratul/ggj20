#include "game_stage.h"
#include "registry.h"
#include "components/sprite_component.h"
#include "components/camera_component.h"
#include "sprite_layers.h"
#include "services/rhythm_service.h"
#include "services/item_service.h"
#include "services/ui_service.h"
#include "game.h"
#include "halley/entity/components/transform_2d_component.h"
#include "title_stage.h"

GameStage::GameStage(std::shared_ptr<ItemService> itemService)
	: itemService(itemService)
{
}

void GameStage::init()
{
	auto& game = dynamic_cast<GGJ20Game&>(getGame());
	
	painterService = std::make_shared<PainterService>();
	rhythmService = std::make_shared<RhythmService>();

	world = createWorld("stages/game_stage");
	world->addService(painterService);
	world->addService(game.getInputService());
	world->addService(rhythmService);
	world->addService(itemService);
	world->addService(std::make_shared<UIService>());

	world->createEntity()
		.addComponent(Transform2DComponent(Vector2f(192.0f, 108.0f)))
		.addComponent(CameraComponent(game.getZoom(), Colour4f(0.0f, 0.0f, 0.0f), 1, 0));
}

void GameStage::onVariableUpdate(Time t)
{
	dynamic_cast<GGJ20Game&>(getGame()).getInputService()->update(t);
	world->step(TimeLine::VariableUpdate, t);

	if (itemService->needsRestart()) {
		getAPI().core->setStage(std::make_unique<TitleStage>());
	}
}

void GameStage::onRender(RenderContext& rc) const
{
	painterService->spritePainter.start();
	world->render(rc);
}
