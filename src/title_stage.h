#pragma once

#include <halley.hpp>
using namespace Halley;

class TitleStage : public EntityStage
{
public:
	void init() override;
	void onVariableUpdate(Time) override;
	void onRender(RenderContext&) const override;

private:
	Sprite titleScreen;
};
