#include <systems/rhythm_system.h>

using namespace Halley;

class RhythmSystem final : public RhythmSystemBase<RhythmSystem> {
public:	
	void update(Time t)
    {
		for (auto& e: mainFamily) {
			e.rhythmArea.elapsed += float(t);
		}
	}
};

REGISTER_SYSTEM(RhythmSystem)

