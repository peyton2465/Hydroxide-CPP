#include <string_view>
#include <iostream>
#include <sstream>
#include <string>

#include <thread>
#include <chrono>

#include <format>
#include <source_location>

#include <Windows.h>

import mem;
import unlink;

import rbx.object.instance;
import rbx.object.script_context;
import rbx.object.task_scheduler;
import rbx.object.data_model;

import rbx.lua.constants;
import rbx.lua.object;
import rbx.lua.api;

import rbx.lua.hash;
import rbx.lua.string;

import rbx.bypass.console;
import rbx.bypass.retcheck;

import kiero;
import directx.hook;

rbx::script_context* script_context = nullptr;

static void output_addresses(rbx::task_scheduler* task_scheduler, rbx::data_model* data_model) {
	std::cout << "Base Address: 0x" << std::hex << reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr)) << std::dec << '\n';
	std::cout << '\n';

	std::cout << "lua_State: 0x" << std::hex << reinterpret_cast<std::uintptr_t>(script_context->get_lua_state()) << std::dec << '\n';

	std::cout << "TaskScheduler: 0x" << std::hex << reinterpret_cast<std::uintptr_t>(task_scheduler) << std::dec << '\n';
	std::cout << "ScriptContext: 0x" << std::hex << reinterpret_cast<std::uintptr_t>(script_context) << std::dec << '\n';
	std::cout << "DataModel: 0x" << std::hex << reinterpret_cast<std::uintptr_t>(data_model) << std::dec << '\n';
	std::cout << '\n';
}

int main() {
	using namespace std::chrono_literals;

	rbx::task_scheduler* task_scheduler = rbx::get_task_scheduler();
	script_context = rbx::get_script_context();
	auto data_model = reinterpret_cast<rbx::data_model*>(script_context->parent);
	
	rbx::bypass::console(L"Hydroxide: Debugging Console");
	output_addresses(task_scheduler, data_model);

	task_scheduler->hook_job();
	task_scheduler->push([]() {
		rbx::state* thread = script_context->get_lua_state();
		rbx::state* new_thread = rbx::new_thread(thread);

		rbx::get_global(new_thread, "table");
		rbx::get_field(new_thread, -1, "getn");
		rbx::set_registry(new_thread, "getn");
		rbx::pop(new_thread, 1);

		rbx::push_value(thread, -1);
		rbx::aux::ref(thread, rbx::registry);

		std::cout << "new_State: " << std::hex << reinterpret_cast<std::uintptr_t>(new_thread) << std::dec << '\n';

		rbx::get_global(new_thread, "game");
		rbx::get_field(new_thread, -1, "Name");
		std::cout << rbx::to_string(new_thread, -1) << '\n';

		//rbx::get_instance_map(new_thread);
		//rbx::set_registry(new_thread, "instances");

		//rbx::push_nil(new_thread);

		//while (rbx::next(new_thread, -2)) {
		//	std::cout << rbx::type(new_thread, -1) << '\n';
		//	rbx::pop(new_thread, 1);
		//}

		rbx::pop(new_thread, 2);
		rbx::pop(thread, 1);
	});
}

bool __stdcall DllMain(HMODULE module_handle, std::uintptr_t reason, const char* directory) {
	const auto path = ([=]() -> std::string {
		if (directory == nullptr) {
			char buffer[MAX_PATH];
			::GetModuleFileNameA(module_handle, buffer, MAX_PATH);

			return std::string(buffer, std::strlen(buffer) - std::strlen("bin/Hydroxide.dll"));
		}

		return directory;
	})();

	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(main).detach();
		std::thread(directx::hook, path).detach();
	}

	::DisableThreadLibraryCalls(module_handle);
	::UnlinkModuleFromPEB(module_handle);

	return true;
}
