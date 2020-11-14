#pragma once
#include <string>

// Necessary items for MessageBox
class MessageBoxConfig {
private:
	struct s_Captain {
		std::string Hint, Error, Warning;
	};
	struct s_Message {
		std::string HookFailed, UnHookFailed, IniNotExist, TerrainDisabled,
			TerrainMapUnloaded, TerrainMapUnknown, TriggerEventFull, TriggerActionFull,
			HouseWndNotFound, INISectionNotFound;
	};
public:
	static MessageBoxConfig Instance;

	MessageBoxConfig() {
		Message.HookFailed = "FA2Copy failed to hook!";
		Message.UnHookFailed = "FA2Copy failed to unhook!";
		Message.IniNotExist = "Cannot read FA2CopyData.ini!";
	}
	s_Captain Captain;
	s_Message Message;
};