#include <systems/rhythm_render_system.h>
#include "src/blacksmith_actions.h"

using namespace Halley;

class RhythmRenderSystem final : public RhythmRenderSystemBase<RhythmRenderSystem> {
public:	
	void render(RenderContext& rc)
    {
		for (auto& c: cameraFamily) {
			auto cam = Camera().setPosition(c.transform2D.getGlobalPosition()).setZoom(c.camera.zoom);
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
		for (auto& e: mainFamily) {
			const float endRadius = 21 * (e.rhythmArea.action == BlacksmithActions::Idle ? 2.0f : 1.0f);
			const float startRadius = endRadius * 3;

			float t = clamp(e.rhythmArea.elapsed / e.rhythmArea.totalTime, 0.0f, 1.0f);
			float radius = lerp(startRadius, endRadius, powf(t, 2.1f));
			float opacity = pow(t, 3.0f);
			auto col = e.rhythmArea.action == BlacksmithActions::Idle ? Colour4f(0.5f, 1.0f, 0.5f, opacity) :  Colour4f(1, 1, 1, opacity);
			auto col2 = col;
			col2.a = 0.35f;
			painter.drawCircle(e.transform2D.getGlobalPosition(), endRadius, 2, col2);
			painter.drawCircle(e.transform2D.getGlobalPosition(), radius, 2, col);
		}
	}
};

REGISTER_SYSTEM(RhythmRenderSystem)

