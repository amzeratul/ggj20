#include <systems/rhythm_render_system.h>

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
			float radius = lerp(startRadius, endRadius, powf(t, 2.5f));
			float opacity = t;
			painter.drawCircle(e.position.position, radius, 2, Colour4f(1, 1, 1, opacity));
		}
	}
};

REGISTER_SYSTEM(RhythmRenderSystem)

