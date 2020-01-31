#pragma once

#include <halley.hpp>
#include "services/painter_service.h"
using namespace Halley;

class GameStage : public EntityStage {
public:
	void init() override;

	void onVariableUpdate(Time) override;
	void onRender(RenderContext&) const override;

private:
	std::shared_ptr<PainterService> painter;
	std::shared_ptr<World> world;	 
};
