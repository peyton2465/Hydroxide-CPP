#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <string>

#include <nlohmann/json.hpp>

#include <unordered_map>

import injection;

std::unordered_map<std::string, int> test{
	{"hello", 1337}
};

int main() {
	if (!inject(std::filesystem::current_path() / "bin" / "Hydroxide.dll", L"RobloxPlayerBeta.exe")) {
		std::cin.get();
	}

	test["wehat"] = 40;

	std::cout << test["hello"] << ' ' << test["wehat"] << '\n';

	return 0;
}