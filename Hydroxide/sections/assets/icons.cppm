module;

#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

#include <d3d11.h>

#include "../../libraries/imgui/imgui.h"

export module oh.assets;

import imgui.misc;



export namespace oh::assets {
	namespace types {
		std::unordered_map<std::string, ID3D11ShaderResourceView*> icons;

		void load_icons(const std::filesystem::path& root_path, ID3D11Device* device) {
			std::size_t width = 16;
			std::size_t height = 16;

			const auto icons_folder = root_path / "assets" / "icons" / "types";

			auto load_icon = [&](std::string_view type_name, std::string_view file_name) {
				icons[type_name.data()] = nullptr;
				LoadTextureFromFile(device, (icons_folder / file_name.data()).string().c_str(), &icons[type_name.data()], &width, &height);
			};

			load_icon("function", "closure.png");
			load_icon("string", "integral.png");
			load_icon("boolean", "integral.png");
			load_icon("number", "integral.png");
			load_icon("table", "table.png");
			load_icon("vector", "userdata.png");
			load_icon("userdata", "userdata.png");
		}
	}

	namespace explorer {
		std::unordered_map<std::string, ID3D11ShaderResourceView*> icons;

		void load_icons(const std::filesystem::path& root_path, ID3D11Device* device) {
			std::size_t width = 16;
			std::size_t height = 16;

			const auto icons_folder = root_path / "assets" / "icons" / "explorer";

			auto load_icon = [&](std::string_view class_name, std::string_view file_name) {
				icons[class_name.data()] = nullptr;
				LoadTextureFromFile(device, (icons_folder / file_name.data()).string().c_str(), &icons[class_name.data()], &width, &height);
			};

			load_icon("Unknown", "unknown.png");

			load_icon("Workspace", "workspace.png");
			load_icon("Players", "players.png");
			load_icon("ReplicatedStorage", "replicated.png");
			load_icon("RobloxReplicatedStorage", "replicated.png");
			load_icon("ReplicatedFirst", "replicated.png");
			load_icon("Lighting", "lighting.png");
			load_icon("ServerScriptService", "server_script_service.png");
			load_icon("ServerStorage", "server_storage.png");
			load_icon("TestService", "test_service.png");
			load_icon("Teams", "teams.png");
			load_icon("StarterPack", "starter_pack.png");
			load_icon("CorePackages", "starter_pack.png");
			load_icon("StarterPlayer", "starter_player.png");
			load_icon("StarterGui", "starter_gui.png");
			load_icon("CoreGui", "starter_gui.png");
			load_icon("SoundService", "sound_service.png");
			load_icon("LocalizationService", "localization_service.png");
			load_icon("Chat", "chat.png");
			load_icon("NetworkClient", "network_client.png");
			load_icon("StarterPack", "starter_pack.png");

			load_icon("StarterPlayerScripts", "scripts.png");
			load_icon("StarterCharacterScripts", "scripts.png");

			load_icon("PlayerScripts", "scripts.png");
			load_icon("PlayerGui", "starter_gui.png");
			load_icon("StarterGear", "starter_pack.png");
			load_icon("Backpack", "starter_pack.png");

			load_icon("ClientReplicator", "client_replicator.png");

			load_icon("Terrain", "terrain.png");

			load_icon("Model", "model.png");

			load_icon("Part", "part.png");
			load_icon("CornerWedgePart", "part.png");
			load_icon("TrussPart", "part.png");
			load_icon("WedgePart", "part.png");
			load_icon("MeshPart", "mesh_part.png");

			load_icon("BlockMesh", "mesh.png");
			load_icon("SpecialMesh", "mesh.png");
			load_icon("CharacterMesh", "mesh.png");

			load_icon("Decal", "decal.png");
			load_icon("Texture", "texture.png");
			load_icon("SurfaceAppearance", "texture.png");

			load_icon("Dialog", "dialog.png");
			load_icon("DialogChoice", "dialog_choice.png");

			load_icon("ClickDetector", "click_detector.png");
			load_icon("ProximityPrompt", "click_detector.png");

			load_icon("ArcHandles", "arc_handles.png");

			load_icon("HandleAdornment", "_handle_adornment.png");
			load_icon("BoxHandleAdornment", "box_handle_adornment.png");
			load_icon("ConeHandleAdornment", "cone_handle_adornment.png");
			load_icon("CylinderHandleAdornment", "cylinder_handle_adornment.png");
			load_icon("ImageHandleAdornment", "image_handle_adornment.png");
			load_icon("LineHandleAdornment", "line_handle_adornment.png");
			load_icon("SphereHandleAdornment", "sphere_handle_adornment.png");

			load_icon("Handles", "handles.png");

			load_icon("SelectionBox", "selection_box.png");
			load_icon("SelectionSphere", "selection_sphere.png");

			load_icon("SurfaceSelection", "surface_selection.png");

			load_icon("Animation", "animation.png");
			load_icon("AnimationController", "animation.png");

			load_icon("Bone", "bone.png");
			load_icon("Motor6D", "motor_6d.png");

			load_icon("Accessory", "accessory.png");
			load_icon("Shirt", "shirt.png");
			load_icon("ShirtGraphic", "shirt_graphic.png");
			load_icon("Pants", "pants.png");

			load_icon("Player", "player.png");
			load_icon("Humanoid", "humanoid.png");
			load_icon("HumanoidDescription", "humanoid_description.png");

			load_icon("Attachment", "attachment.png");

			load_icon("AlignOrientation", "align_orientation.png");
			load_icon("AlignPosition", "align_position.png");

			load_icon("AngularVelocity", "torque.png");

			load_icon("BallSocketConstraint", "ball_socket_constraint.png");
			load_icon("CylindricalConstraint", "cylindrical_constraint.png");
			load_icon("HingeConstraint", "hinge_constraint.png");
			load_icon("PrismaticConstraint", "prismatic_constraint.png");
			load_icon("RodConstraint", "rod_constraint.png");
			load_icon("RopeConstraint", "rope_constraint.png");
			load_icon("SpringConstraint", "spring_constraint.png");
			load_icon("WeldConstraint", "weld_constraint.png");

			load_icon("Torque", "torque.png");
			load_icon("VectorForce", "vector_force.png");

			load_icon("Beam", "beam.png");
			load_icon("Explosion", "explosion.png");
			load_icon("Fire", "fire.png");
			load_icon("ParticleEmitter", "particle_emitter.png");
			load_icon("Smoke", "smoke.png");
			load_icon("Sparkles", "sparkles.png");
			load_icon("Trail", "trail.png");

			load_icon("Sky", "sky.png");
			load_icon("Atmosphere", "atmosphere.png");

			load_icon("BillboardGui", "billboard_gui.png");
			load_icon("Frame", "frame.png");
			load_icon("ScrollingFrame", "frame.png");
			load_icon("ImageButton", "image_button.png");
			load_icon("ImageLabel", "image_label.png");
			load_icon("ScreenGui", "screen_gui.png");
			load_icon("SurfaceGui", "billboard_gui.png");
			load_icon("TextLabel", "text_label.png");
			load_icon("TextButton", "text_button.png");
			load_icon("TextBox", "text_button.png");

			load_icon("VideoFrame", "video_frame.png");
			load_icon("ViewportFrame", "image_button.png");

			load_icon("UIAspectRatioConstraint", "ui.png");
			load_icon("UICorner", "ui.png");
			load_icon("UIGradient", "ui.png");
			load_icon("UIGridLayout", "ui.png");
			load_icon("UIListLayout", "ui.png");
			load_icon("UIPadding", "ui.png");
			load_icon("UIPageLayout", "ui.png");
			load_icon("UISizeConstraint", "ui.png");
			load_icon("UITableLayout", "ui.png");
			load_icon("UITextSizeConstraint", "ui.png");

			load_icon("BodyAngularVelocity", "body.png");
			load_icon("BodyForce", "body.png");
			load_icon("BodyGyro", "body.png");
			load_icon("BodyPosition", "body.png");
			load_icon("BodyThrust", "body.png");
			load_icon("BodyVelocity", "body.png");
			load_icon("RocketPropulsion", "body.png");

			load_icon("Tool", "tool.png");
			load_icon("Seat", "seat.png");
			load_icon("VehicleSeat", "seat.png");
			load_icon("SpawnLocation", "spawn.png");
			load_icon("Team", "team.png");

			load_icon("PointLight", "lighting.png");
			load_icon("SpotLight", "lighting.png");
			load_icon("SurfaceLight", "lighting.png");

			load_icon("LocalizationTable", "localization_table.png");

			load_icon("BloomEffect", "effect.png");
			load_icon("BlurEffect", "effect.png");
			load_icon("ColorCorrectionEffect", "effect.png");
			load_icon("DepthOfFieldEffect", "effect.png");
			load_icon("SunRaysEffect", "effect.png");

			load_icon("Script", "script.png");
			load_icon("LocalScript", "local_script.png");
			load_icon("ModuleScript", "module_script.png");

			load_icon("BindableEvent", "bindable_event.png");
			load_icon("BindableFunction", "bindable_function.png");
			load_icon("RemoteEvent", "remote_event.png");
			load_icon("RemoteFunction", "remote_function.png");

			load_icon("Sound", "sound.png");
			load_icon("SoundIdGroup", "sound_group.png");
			load_icon("SoundGroup", "sound_group.png");

			load_icon("ChorusSoundEffect", "sound_effect.png");
			load_icon("CompressorSoundEffect", "sound_effect.png");
			load_icon("DistortionSoundEffect", "sound_effect.png");
			load_icon("EchoSoundEffect", "sound_effect.png");
			load_icon("EqualizerSoundEffect", "sound_effect.png");
			load_icon("FlangeSoundEffect", "sound_effect.png");
			load_icon("PitchShiftSoundEffect", "sound_effect.png");
			load_icon("ReverbSoundEffect", "sound_effect.png");
			load_icon("TremoloSoundEffect", "sound_effect.png");

			load_icon("Camera", "camera.png");
			load_icon("Configuration", "configuration.png");
			load_icon("Folder", "folder.png");
			load_icon("Snap", "weld.png");
			load_icon("Weld", "weld.png");

			load_icon("BoolValue", "value.png");
			load_icon("BrickColorValue", "value.png");
			load_icon("CFrameValue", "value.png");
			load_icon("IntValue", "value.png");
			load_icon("Color3Value", "value.png");
			load_icon("NumberValue", "value.png");
			load_icon("ObjectValue", "value.png");
			load_icon("RayValue", "value.png");
			load_icon("StringValue", "value.png");
			load_icon("Vector3Value", "value.png");
		}
	}
}
