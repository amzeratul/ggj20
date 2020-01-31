#include <systems/camera_system.h>

using namespace Halley;

class CameraSystem final : public CameraSystemBase<CameraSystem> {
public:	
	void update(Time t) {
	}
};

REGISTER_SYSTEM(CameraSystem)

