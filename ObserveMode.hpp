#pragma once

#include "Mode.hpp"
#include "Scene.hpp"
#include "Sound.hpp"


struct Grunt {

	Scene::Transform* parent = 0; //The parent of all the parts;
	Scene::Transform* left_leg = 0;
	Scene::Transform* right_leg = 0;
	Scene::Transform* right_arm = 0;
	Scene::Transform* left_arm = 0;
	Scene::Transform* body = 0;
	Scene::Transform* head = 0;


	/*------ WALKING INFO ------*/
	bool leg_up = true; //Going up or down?
	float time_up = 0.0f; // How long a leg has been up in the air
	float walking_speed = 0.75f;
	bool left_leg_up = true; //Which leg I'm using


	void handle_walking(float elapsed) { //Ugly so I put it here
		if (left_leg_up) {
			if (leg_up) {
				time_up += elapsed;
				left_leg->position += glm::vec3(0.0f, 0.0f, 1.0f * elapsed);
				if (time_up > walking_speed) {
					leg_up = false;
					time_up = 0.0f;
				}
			}
			else {
				time_up += elapsed;
				left_leg->position += glm::vec3(0.0f, 0.0f, -1.0f * elapsed);
				if (time_up > walking_speed) {
					leg_up = true;
					time_up = 0.0f;
					left_leg_up = false;
				}
			}
		}
		else {
			if (leg_up) {
				time_up += elapsed;
				right_leg->position += glm::vec3(0.0f, 0.0f, 1.0f * elapsed);
				if (time_up > walking_speed) {
					leg_up = false;
					time_up = 0.0f;
				}
			}
			else {
				time_up += elapsed;
				right_leg->position += glm::vec3(0.0f, 0.0f, -1.0f * elapsed);
				if (time_up > walking_speed) {
					leg_up = true;
					time_up = 0.0f;
					left_leg_up = true;
				}
			}
		}
	}
};


struct ObserveMode : Mode {
	ObserveMode();
	virtual ~ObserveMode();

	void create_clone(glm::vec3 offset, Grunt grunt);
	std::string append_clone_number(std::string name);

	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	Scene::Camera const *current_camera = nullptr;

	std::vector<Grunt> grunts;
	Grunt og;
	int clone_number = 1;
	bool in_scope = false; //You know what else isn't in scope? This homework...

	glm::vec2 ul = glm::vec2(166, 115);
	std::string armature = "Armature";
	std::string left_leg = "LegLeft";
	std::string right_leg = "LegRight";
	std::string left_arm = "ArmLeft";
	std::string right_arm = "ArmRight";
	std::string body = "Body";
	std::string head = "Head";


	/* ---------- GAME STATE ---------- */
	bool in_intro = true;
	bool talking = true;
	bool in_ending = false;
	float gap = 0.0;

	std::shared_ptr< Sound::PlayingSample > play_ring = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_col_line_1 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_col_line_2 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_col_line_3 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_col_good_job = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_snack_line_1 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_snack_line_2 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_snack_line_3 = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_snack_controls = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_sniper_shot = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_set_up = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_people_same = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_random_person = nullptr;
	std::shared_ptr< Sound::PlayingSample > play_boots_deactivate = nullptr;

};

