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
			float radius = lerp(startRadius, endRadius, powf(t, 2.1f));
			float opacity = pow(t, 2.0f);
			painter.drawCircle(e.position.position, radius, 2, Colour4f(1, 1, 1, opacity));
		}

		BlacksmithActions actions[] = {BlacksmithActions::Anvil, BlacksmithActions::Bucket, BlacksmithActions::Furnace, BlacksmithActions::Love};
		for (auto& a: actions) {
			painter.drawCircle(BlacksmithActionsUtils::actionToPos(a), endRadius, 2, Colour4f(1, 1, 1, 0.1f));
		}
	}
};

REGISTER_SYSTEM(RhythmRenderSystem)

