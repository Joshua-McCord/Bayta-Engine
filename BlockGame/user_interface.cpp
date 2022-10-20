#include "user_interface.h"
#include "bezier.hpp"

User_Interface::User_Interface(Level* level, Renderer* renderer)
{
	this->level = level;
	this->renderer = renderer;
}

void User_Interface::draw_ui()
{
	// Start a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Draw UI 
	draw_player_options();
	draw_water_options();
	draw_world_grid_options();
	draw_system_options();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void User_Interface::draw_player_options()
{
	ImGui::Begin("Player Controls");
	ImGui::SetWindowFontScale(1.5f);

	//if (ImGui::MenuItem("Save", "Ctrl+S")) { renderer->player.save(); }
	ImGui::Bezier("Anim Curve", renderer->player.animation_curve);
	ImGui::SliderFloat("Speed", &renderer->player.animation_speed, 0.0f, 0.5f);


	if (ImGui::Button("Save")) {}
	ImGui::End();

}

void User_Interface::draw_world_grid_options()
{

}

void User_Interface::draw_water_options()
{
	bool p = true;
	bool* p_open = &p;

	Water* water = &renderer->water;


	GerstnerMaterial* gerstner_material = &water->gerstner;
	ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Water", p_open, ImGuiWindowFlags_MenuBar))
	{
		ImGui::SetWindowFontScale(1.5f);
		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for (int i = 0; i < gerstner_material->num_waves; ++i)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				char label[128];
				sprintf_s(label, "Wave %d", i + 1);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Wave %d", selected);
			ImGui::SliderFloat2("Dir.", &gerstner_material->waves[selected].direction.x, -1, 1);
			ImGui::SliderFloat("Ampl.", &gerstner_material->waves[selected].amplitude, 0, 1);
			ImGui::SliderFloat("Freq.", &gerstner_material->waves[selected].frequency, 0, 1);
			ImGui::SliderFloat("Spd.", &gerstner_material->waves[selected].speed, -5, 5);
			ImGui::SliderFloat("Stpns.", &gerstner_material->waves[selected].steepness, 0, 5);
		}
		ImGui::Separator();
		{
			//
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::ColorPicker3("Diffuse", &water->material.diffuse.x);
			ImGui::ColorPicker3("Specular", &water->material.specular.x);
			ImGui::ColorPicker3("Ambient", &water->material.ambient.x);
			ImGui::SliderFloat("Shininess", &water->material.shininess, 0, 5);
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}

void User_Interface::draw_system_options()
{
	ImGui::Begin("System");
	ImGui::SetWindowFontScale(1.5f);
	ImGui::Checkbox("Wireframe Rendering", &renderer->wireframe);
	if (ImGui::Button("Save")) { renderer->save(); }
	ImGui::End();
}