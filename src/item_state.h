#pragma once

enum class ItemState {
    QueueBack,
    QueueFront,
    CurrentWait,
    CurrentActive,
    Out,
	Dead
};
