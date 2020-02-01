#pragma once

enum class ItemState {
	QueuePre,
    QueueBack,
    QueueFront,
    CurrentWait,
    CurrentActive,
	Done,
    Out,
	Dead
};
