module;

#include <string_view>
#include <string>
#include <memory>
#include <vector>
#include <format>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <iostream>

#include <d3d11.h>

#include <nlohmann/json.hpp>

#include "../libraries/imgui/imgui.h"
#include "../libraries/imgui/imgui_internal.h"

export module oh.explorer;

import rbx.object.instance;
import rbx.object.script_context;
import rbx.object.task_scheduler;
import rbx.object.data_model;

import rbx.lua.constants;
import rbx.lua.object;
import rbx.lua.api;
import rbx.lua.string;
import rbx.lua.hash;
import rbx.lua.gc;

import file;

import imgui.misc;

import oh.assets;

struct property {
	std::string name;
	std::string category;
	struct security_t {
		std::string read;
		std::string write;
	} security;
	std::string value;
};

static rbx::instance* selected_instance = nullptr;
static rbx::userdata* instance_userdata = nullptr;
static std::unordered_map<std::string, std::tuple<std::string, std::vector<property>, std::vector<std::string>>> mapped_properties;
static std::vector<std::string> current_properties;
static std::unordered_map<std::string, std::string> current_values;

void load_properties(const std::string_view& class_name) {
	const auto& [super_class, properties, events] = mapped_properties[class_name.data()];
	rbx::task_scheduler* task_scheduler = rbx::get_task_scheduler();

	if (super_class != "<<<ROOT>>>") {
		load_properties(super_class.c_str());
	}

	for (const auto& prop : properties) {
		current_properties.push_back(prop.name);
	}
};

void render_instance(auto instance, const std::string_view& name = "") {
	auto instance_ptr = ([=]() constexpr {
		if constexpr (std::is_same_v<decltype(instance), rbx::instance*>) {
			return instance;
		} else if constexpr (std::is_same_v<decltype(instance), std::shared_ptr<rbx::instance>>) {
			return instance.get();
		}
	})();

	const auto& icons = oh::assets::explorer::icons;
	bool opened = false;

	if (instance->children) {
		opened = ImGui::TreeNodeEx(instance_ptr, 0, "");
		ImGui::SameLine();
	} else {
		ImGui::Indent();
	}

	if (icons.contains(instance->descriptor.class_name)) {
		ImGui::Image(icons.at(instance->descriptor.class_name), ImVec2{ 16.0f, 16.0f });
	} else {
		ImGui::Image(icons.at("Unknown"), ImVec2{ 16.0f, 16.0f });
	}

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });

	if (ImGui::SmallButton(name.empty() ? instance->name.c_str() : name.data())) {
		const auto task_scheduler = rbx::get_task_scheduler();
		selected_instance = instance_ptr;
		
		task_scheduler->push([]() {
			const auto thread = rbx::get_script_context()->get_lua_state();

			if (selected_instance->parent == rbx::get_script_context()->parent) {
				rbx::get_global(thread, "game");
				rbx::get_field(thread, -1, "GetService");
				rbx::get_global(thread, "game");
				rbx::push_string(thread, selected_instance->descriptor.class_name.c_str());
				rbx::pcall(thread, 2, 1);

				instance_userdata = &rbx::index2adr(thread, -1)->value.gc->userdata;

				rbx::pop(thread, 2);
			} else {
				const auto global_state = rbx::get_global_state(thread);
				rbx::gcobject* current_object = global_state->rootgc;

				while (current_object) {
					if (current_object->gch.type == rbx::userdata_t) {
						auto userdata = &current_object->userdata;
						if (auto meta_table = reinterpret_cast<rbx::table*>(reinterpret_cast<std::uintptr_t>(userdata->metatable) - reinterpret_cast<std::uintptr_t>(&userdata->metatable))) {
							if (auto type = rbx::hash::get_string(meta_table, rbx::str::new_string(thread, "__type")); type->type == rbx::string_t && type->value.gc->string == "Instance") {
								if (*reinterpret_cast<rbx::instance**>(userdata + 1) == selected_instance) {
									instance_userdata = userdata;
									break;
								}
							}
						}
					}

					current_object = current_object->gch.next;
				}
			}

			if (!current_properties.empty()) {
				current_properties.clear();
			}

			load_properties(selected_instance->descriptor.class_name.c_str());
		});
	}

	ImGui::PopStyleColor();

	if (instance->children && opened) {
		for (const auto& child : *instance) {
			render_instance(child);
		}

		ImGui::TreePop();
	} else if (!instance->children) {
		ImGui::Unindent();
	}
}



export namespace oh::explorer {
	void initialize(const std::filesystem::path& root_path) {
		using nlohmann::json;

		const auto instances = json::parse(file::read(root_path / "assets" / "properties.json"));

		for (const auto& [class_name, object] : instances.items()) {
			std::vector<property> properties;
			properties.reserve(object["members"].size());

			for (const auto member: object["members"]) {
				properties.emplace_back(
					member["name"].get<std::string>(),
					member["category"].get<std::string>(),
					property::security_t{
						member["security"]["read"].get<std::string>(),
						member["security"]["write"].get<std::string>(),
					},
					member["value"].get<std::string>()
				);
			}
			
			mapped_properties[class_name] = std::make_tuple(object["class"].get<std::string>(), properties, object["events"].get<std::vector<std::string>>());
		}
	}

	void render() {
		auto script_context = rbx::get_script_context(rbx::scan_option::scan_once);
		auto state = script_context->get_lua_state();
		auto global_state = rbx::get_global_state(state);
		auto game = reinterpret_cast<rbx::data_model*>(script_context->parent);

		ImGui::Begin("Hydroxide: Instance Explorer");

		ImGui::BeginGroup();

		static bool show_empty_services = false;
		ImGui::Checkbox("View empty services", &show_empty_services);

		if (ImGui::TreeNodeEx("Nil Instances")) {
			rbx::gcobject* current_object = global_state->rootgc;

			while (current_object) {
				if (current_object->gch.type == rbx::userdata_t) {
					auto userdata = &current_object->userdata;
					if (auto meta_table = reinterpret_cast<rbx::table*>(reinterpret_cast<std::uintptr_t>(userdata->metatable) - reinterpret_cast<std::uintptr_t>(&userdata->metatable))) {
						if (auto type = rbx::hash::get_string(meta_table, rbx::str::new_string(state, "__type")); type->type == rbx::string_t && type->value.gc->string == "Instance") {
							if (auto instance = *reinterpret_cast<rbx::instance**>(userdata + 1); instance->parent == nullptr) {
								render_instance(instance);
							}
						}
					}
				}

				current_object = current_object->gch.next;
			}

			ImGui::TreePop();
		}

		for (auto& child : *game) {
			if (child->children || show_empty_services) {
				render_instance(child, child->descriptor.class_name);
			}
		}

		ImGui::EndGroup();

		ImGui::Separator();

		if (selected_instance != nullptr) {
			if (!current_properties.empty()) {
				for (const auto& property : current_properties) {
					ImGui::Text(property.c_str());
					ImGui::SameLine();
					ImGui::Text(current_values[property].c_str());
				}
			}

			rbx::get_task_scheduler()->push([&]() {
				auto thread = rbx::get_script_context()->get_lua_state();
				auto new_thread = rbx::new_thread(thread);

				rbx::set_userdata_value(new_thread->top, instance_userdata);
				rbx::incr_top(new_thread);

				for (const auto& property : current_properties) {
					std::cout << std::format("getting: {}", property) << '\n';
					auto tong = rbx::index2adr(new_thread, -1);
					rbx::get_field(new_thread, -1, property.c_str());

					auto tang = rbx::index2adr(new_thread, -1);
					
					switch (rbx::type(new_thread, -1)) {
					case rbx::nil_t: {
						current_values[property] = "nil";
						break;
					}
					case rbx::number_t: {
						current_values[property] = std::to_string(rbx::to_number(new_thread, -1));
						break;
					}
					case rbx::boolean_t: {
						current_values[property] = rbx::to_boolean(new_thread, -1) ? "true" : "false";
						break;
					}
					case rbx::vector_t: {
						const auto [x, y, z] = rbx::to_vector(new_thread, -1);

						current_values[property] = std::format("{}, {}, {}", x, y, z);
						break;
					}
					case rbx::string_t: {
						std::cout << "pushing string\n";
						current_values[property] = rbx::to_string(new_thread, -1).data();
						std::cout << "pushed string\n";
						break;
					}
					case rbx::userdata_t: {
						auto userdata = &rbx::index2adr(new_thread, -1)->value.gc->userdata;

						if (auto meta_table = reinterpret_cast<rbx::table*>(reinterpret_cast<std::uintptr_t>(userdata->metatable) - reinterpret_cast<std::uintptr_t>(&userdata->metatable))) {
							if (auto type = rbx::hash::get_string(meta_table, rbx::str::new_string(new_thread, "__type")); type->type == rbx::string_t) {
								rbx::string& string = type->value.gc->string;

								if (string == "Instance") {
									current_values[property] = (*reinterpret_cast<rbx::instance**>(userdata + 1))->name;
								}
								else {
									current_values[property] = "";
								}
							}
						}

						break;
					}
					}

					std::cout << "removing property from stack\n";
					rbx::pop(new_thread, 1);
				}

				std::cout << "removing instance userdata from stack\n";
				rbx::pop(new_thread, 1);
				std::cout << "removing thread from main thread\n";
				rbx::pop(thread, 1);
				std::cout << "finished one iteration\n";
			});
		}

		ImGui::End();
	}
}
