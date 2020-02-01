#include <systems/rhythm_render_system.h>
#include "src/blacksmith_actions.h"

using namespace Halley;

class RhythmRenderSystem final : public RhythmRenderSystemBase<RhythmRenderSystem> {
public:	
	void render(RenderContext& rc)
    {
		for (auto& c: cameraFamily) {
			auto cam = Camera().setPosition(c.position.position).setZoom(c.camera.zoom);
			rc.with(cam).bind([&] (Painter& painter)
			{
				paint(painter);
			});
			break;
		}
	}

private:
	void paint(Painter& painter)
	{
		const float endRadius = 21;
		const float startRadius = endRadius * 3;
		
		for (auto& e: mainFamily) {
			float t = clamp(e.rhythmArea.elapsed / e.rhythmArea.totalTime, 0.0f, 1.0f);
			float radius = lerp(startRadius, endRadius, powf(t, 2.1f)) * (e.rhythmArea.action == BlacksmithActions::Idle ? 2.0f : 1.0f);
			float opacity = pow(t, 2.0f);
			auto col = e.rhythmArea.action == BlacksmithActions::Idle ? Colour4f(0.5f, 1.0f, 0.5f, opacity) :  Colour4f(1, 1, 1, opacity);
			painter.drawCircle(e.position.position, radius, 2, col);
		}

		BlacksmithActions actions[] = {BlacksmithActions::Anvil, BlacksmithActions::Bucket, BlacksmithActions::Furnace, BlacksmithActions::Love, BlacksmithActions::Idle};
		for (auto& a: actions) {
			auto col = a == BlacksmithActions::Idle ? Colour4f(0.5f, 1.0f, 0.5f, 0.2f) :  Colour4f(1, 1, 1, 0.2f);
			painter.drawCircle(BlacksmithActionsUtils::actionToPos(a), endRadius * (a == BlacksmithActions::Idle ? 2.0f : 1.0f), 2, col);
		}
	}
};

REGISTER_SYSTEM(RhythmRenderSystem)

