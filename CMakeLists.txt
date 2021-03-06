cmake_minimum_required (VERSION 3.0)

project (ggj20)

set(HALLEY_PROJECT_EMBED 1)
#set(HALLEY_PATH "" CACHE FILEPATH "Path to the Halley root")
set(HALLEY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/halley)
set(HOTRELOAD 0 CACHE BOOL "Sets whether to hot-reload this project")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${HALLEY_PATH}/cmake/")
include(HalleyProject)

if(NOT (DEFINED GGJ20_BIN_DIR))
	set(GGJ20_BIN_DIR "bin")
endif()

set (SOURCES
	"prec.cpp"

	"src/item_config.cpp"
	"src/game.cpp"
	"src/game_stage.cpp"
	"src/title_stage.cpp"

	"src/services/input_service.cpp"
	"src/services/item_service.cpp"
	"src/services/painter_service.cpp"
	"src/services/rhythm_service.cpp"
	"src/services/ui_service.cpp"
	)

set (HEADERS
	"prec.h"

	"src/blacksmith_actions.h"
	"src/item_config.h"
	"src/item_state.h"
	"src/game.h"
	"src/game_stage.h"
	"src/game_state.h"
	"src/move_type.h"
	"src/sprite_layers.h"
	"src/title_stage.h"

	"src/services/input_service.h"
	"src/services/item_service.h"
	"src/services/painter_service.h"
	"src/services/rhythm_service.h"
	"src/services/ui_service.h"
	)

set (GEN_DEFINITIONS
	)

if (MSVC)
	set (HEADERS
		${HEADERS}
		"res/resource.h"
		"res/ggj20.rc"
		"res/ggj20.ico"
	)
endif ()

halleyProject(ggj20 "${SOURCES}" "${HEADERS}" "${GEN_DEFINITIONS}" ${CMAKE_CURRENT_SOURCE_DIR}/${GGJ20_BIN_DIR})
