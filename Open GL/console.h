#pragma once
#include "pch.h"
#include <memory>

namespace Cheat {
	enum LogType {
		Error,
		Debug,
		Success,
	};

	class Console {
	public:
		void InitConsole();
		void Log(LogType, const char*);
	};
	inline std::unique_ptr<Console> g_Console;
}