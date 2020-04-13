#include <systems/particle_system.h>

using namespace Halley;

class ParticleSystem final : public ParticleSystemBase<ParticleSystem> {
public:	
	void update(Time t)
    {
        for (auto& p: particleFamily) {
			p.transform2D.getGlobalPosition() += p.particle.vel * t;
        	if (!p.spriteAnimation.player.isPlaying()) {
				getWorld().destroyEntity(p.entityId);
        	}
        }
	}
};

REGISTER_SYSTEM(ParticleSystem)

