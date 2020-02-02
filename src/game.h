#pragma once

#include <halley.hpp>
#include "services/input_service.h"
using namespace Halley;

class GGJ20Game : public Game {
public:
	void init(const Environment& env, const Vector<String>& args) override;
	int initPlugins(IPluginRegistry& registry) override;
	void initResourceLocator(const Path& gamePath, const Path& assetsPath, const Path& unpackedAssetsPath, ResourceLocator& locator) override;

	String getName() const override;
	String getDataPath() const override;
	bool isDevMode() const override;
	std::unique_ptr<Stage> startGame(const HalleyAPI* api) override;

	std::shared_ptr<InputService> getInputService() const;
	float getZoom() const;

	bool shouldCreateSeparateConsole() const override;

private:
	const HalleyAPI* api = nullptr;
	float zoom = 3;
	
	std::shared_ptr<InputService> inputService;
};
