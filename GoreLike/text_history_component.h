#pragma once

#include <vector>
#include <string>


struct text_history_component
{
	//Full screen not etc
	unsigned short state = 0;

	//All message history don't know when I'll cut it off for now but it'll render as much as it can depending on state
	std::vector<std::string> messages;
};

