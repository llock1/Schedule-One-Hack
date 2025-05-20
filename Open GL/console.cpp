#include "pch.h"
#include "console.h"
#include <format>

namespace Cheat {

	void Console::InitConsole() {
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		Log(LogType::Success, "Allocated Console");
	}

	void Console::Log(LogType logType, const char* message) {
		std::string prefix;

		switch (logType) {
			case LogType::Success:
				prefix = "[SUCCESS]";
				break;
			case LogType::Debug:
				prefix = "[DEBUG]";
				break;
			case LogType::Error:
				prefix = "[ERROR]";
				break;
		}

		std::string formatted_message = std::format("{} {} \n", prefix, message);
		printf(formatted_message.c_str());
	}
}