#include "ObserveMode.hpp"
#include "DrawLines.hpp"
#include "LitColorTextureProgram.hpp"
#include "Mesh.hpp"
#include "Sprite.hpp"
#include "DrawSprites.hpp"
#include "data_path.hpp"
#include "Sound.hpp"
#include "load_save_png.hpp"

#include <iostream>

Sprite const* scope = nullptr;
Sprite const* background = nullptr;


Load< SpriteAtlas > trade_font_atlas(LoadTagDefault, []() -> SpriteAtlas const * {
	SpriteAtlas const* ret = new SpriteAtlas(data_path("the-planet"));
	scope = &ret->lookup("scope");
	background = &ret->lookup("background");
	return ret;
});



GLuint meshes_for_lit_color_texture_program = 0;
static Load< MeshBuffer > meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer *ret = new MeshBuffer(data_path("city.pnct"));
	meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > scene(LoadTagLate, []() -> Scene * {
	Scene *ret = new Scene();

	ret->load(data_path("city.scene"), [](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		auto &mesh = meshes->lookup(mesh_name);
		scene.drawables.emplace_back(transform);
		Scene::Drawable::Pipeline &pipeline = scene.drawables.back().pipeline;

		pipeline = lit_color_texture_program_pipeline;
		pipeline.vao = meshes_for_lit_color_texture_program;
		pipeline.type = mesh.type;
		pipeline.start = mesh.start;
		pipeline.count = mesh.count;

	});

	return ret;
});

Load< Sound::Sample > sniper_shot(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("sniper_shot.opus"));
	});

Load< Sound::Sample > snack_line_1(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("snack_line_1.opus"));
	});

Load< Sound::Sample > snack_line_2(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("snack_line_2.opus"));
	});

Load< Sound::Sample > snack_line_3(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("snack_line_3.opus"));
	});

Load< Sound::Sample > snack_controls(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("snack_controls.opus"));
	});

Load< Sound::Sample > set_up(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("set_up.opus"));
	});

Load< Sound::Sample > ring(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("ring.opus"));
	});

Load< Sound::Sample > random_person(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("random_person.opus"));
	});

Load< Sound::Sample > people_same(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("people_same.opus"));
	});

Load< Sound::Sample > col_line_1(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("col_line_1.opus"));
	});

Load< Sound::Sample > col_line_2(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("col_line_2.opus"));
	});

Load< Sound::Sample > col_line_3(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("col_line_3.opus"));
	});

Load< Sound::Sample > col_good_job(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("col_good_job.opus"));
	});

Load< Sound::Sample > boots_deactivate(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("boots_deactivate.opus"));
	});

ObserveMode::ObserveMode() {

	assert(scene->cameras.size() && "Observe requires cameras.");

	current_camera = &scene->cameras.front();
}

ObserveMode::~ObserveMode() {

}

bool ObserveMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (evt.button.button == SDL_BUTTON_RIGHT && !talking) {
			in_scope = !in_scope;
		}
		if (evt.button.button == SDL_BUTTON_LEFT && !talking && play_random_person && play_random_person->stopped) {
			in_ending = true;
		}
	} else if (evt.type == SDL_MOUSEBUTTONUP) {
		if (evt.button.button == SDL_BUTTON_LEFT) {
			//current_camera->aspect =
		}
	}

	if (evt.type == SDL_MOUSEMOTION && in_scope) {
		//figure out the motion (as a fraction of a normalized [-a,a]x[-1,1] window):
		glm::vec2 delta;
		delta.x = evt.motion.xrel / float(window_size.x) ;
		delta.x *= float(window_size.y) / float(window_size.x);
		delta.y = evt.motion.yrel / float(window_size.y) * -1;

		current_camera->transform->rotation += glm::quat(delta.x, 0, delta.y, 0);
	}
	
	return false;
}

void ObserveMode::create_clone(glm::vec3 offset, Grunt grunt) {

	auto assign_values = [](Scene::Transform* trans, std::string name_, glm::vec3 position_, 
		glm::quat rotation_, glm::vec3 scale_, Scene::Transform* parent_) {

			trans->name = name_;
			trans->position = position_;
			trans->rotation = rotation_;
			trans->scale = scale_;
			trans->parent = parent_;
	};

	
	Scene::Transform* n_parent = new Scene::Transform();
	Scene::Transform* n_head = new Scene::Transform();
	Scene::Transform* n_body = new Scene::Transform();
	Scene::Transform* n_left_arm = new Scene::Transform();
	Scene::Transform* n_right_arm = new Scene::Transform();
	Scene::Transform* n_left_leg = new Scene::Transform();
	Scene::Transform* n_right_leg = new Scene::Transform();


	assign_values(n_parent, append_clone_number(armature), grunt.parent->position + offset,
		grunt.parent->rotation, grunt.parent->scale, grunt.parent->parent);
	assign_values(n_head, append_clone_number(head), grunt.head->position,
		grunt.head->rotation, grunt.head->scale, n_parent);
	assign_values(n_body, append_clone_number(body), grunt.body->position,
		grunt.body->rotation, grunt.body->scale, n_parent);
	assign_values(n_left_arm, append_clone_number(left_arm), grunt.left_arm->position,
		grunt.left_arm->rotation, grunt.left_arm->scale, n_parent);
	assign_values(n_right_arm, append_clone_number(right_arm), grunt.right_arm->position,
		grunt.right_arm->rotation, grunt.right_arm->scale, n_parent);
	assign_values(n_left_leg, append_clone_number(left_leg), grunt.left_leg->position,
		grunt.left_leg->rotation, grunt.left_leg->scale, n_parent);
	assign_values(n_right_leg, append_clone_number(right_leg), grunt.right_leg->position,
		grunt.right_leg->rotation, grunt.right_leg->scale, n_parent);


	std::cout << n_parent << std::endl;
	std::cout << n_parent->name << std::endl;
	//scene->transforms.clear();
}

std::string ObserveMode::append_clone_number(std::string name) {
	return name + std::to_string(clone_number);
}

void ObserveMode::update(float elapsed) {

	if (in_intro) {
		if (!play_ring) {
			play_ring = Sound::play(*ring);
			play_ring->set_volume(1.0f);
		} else if (play_ring && play_ring->stopped && !play_col_line_1) {
			play_col_line_1 = Sound::play(*col_line_1);
			play_col_line_1->set_volume(1.0f);
		} else if (play_col_line_1 && play_col_line_1->stopped && !play_snack_line_1) {
			play_snack_line_1 = Sound::play(*snack_line_1);
			play_snack_line_1->set_volume(1.0f);
		} else if (play_snack_line_1 && play_snack_line_1->stopped && !play_col_line_2) {
			play_col_line_2 = Sound::play(*col_line_2);
			play_col_line_2->set_volume(1.0f);
		} else if (play_col_line_2 && play_col_line_2->stopped && !play_snack_line_2) {
			play_snack_line_2 = Sound::play(*snack_line_2);
			play_snack_line_2->set_volume(1.0f);
		} else if (play_snack_line_2 && play_snack_line_2->stopped && !play_col_line_3) {
			play_col_line_3 = Sound::play(*col_line_3);
			play_col_line_3->set_volume(1.0f);
		} else if (play_col_line_3 && play_col_line_3->stopped && !play_snack_line_3) {
			play_snack_line_3 = Sound::play(*snack_line_3);
			play_snack_line_3->set_volume(1.0f);
		} else if (play_snack_line_3 && play_snack_line_3->stopped) {
			in_intro = false;
		}

		return;
	}



	if (!play_people_same) {
		gap -= elapsed;

		if (gap < 0) {
			play_people_same = Sound::play(*people_same);
			play_people_same->set_volume(1.0f);
			gap = 2.0f;
		}
	} else if (play_people_same && play_people_same->stopped && !play_set_up) {
		gap -= elapsed;

		if (gap < 0) {
			play_set_up = Sound::play(*set_up);
			play_set_up->set_volume(1.0f);
		}
	} else if (play_set_up && play_set_up->stopped && !in_scope && !play_snack_controls) {
		talking = false;

	} else if (play_set_up && play_set_up->stopped && in_scope && !play_snack_controls) {
		talking = true;
		play_snack_controls = Sound::play(*snack_controls);
		play_snack_controls->set_volume(1.0f);
		gap = 2.0f;

	}
	else if (play_snack_controls && play_snack_controls->stopped && !play_random_person) {
		gap -= elapsed;

		if (gap < 0) {
			play_random_person = Sound::play(*random_person);
			play_random_person->set_volume(1.0f);
			talking = false;
		}
	}  else if (in_ending && !play_sniper_shot) {
		play_sniper_shot = Sound::play(*sniper_shot);
		play_sniper_shot->set_volume(1.0f);
	} else if (play_sniper_shot && play_sniper_shot->stopped && !play_col_good_job) {
		play_col_good_job = Sound::play(*col_good_job);
		play_col_good_job->set_volume(1.0f);
	} else if (play_col_good_job && play_col_good_job->stopped) {
		exit(0);
	}


	auto fetch_transform = [this](std::string name, Scene::Transform ** poly) {
		if (*poly != 0) { return; }
		for (int i = 0; i < scene->drawables.size(); i++) {
			std::list<Scene::Transform>::const_iterator it = scene->transforms.begin();
			std::advance(it, i);
			if (it->name == name) {
				std::cout << "Success! Fetched " << name << std::endl;
				*poly = const_cast<Scene::Transform * >(&(*it));
				return;
			}
		}
		std::cout << "Failed to fetch " << name << std::endl;
		exit(0);
	};


	fetch_transform(armature, &og.parent);
	fetch_transform(append_clone_number(left_leg), &og.left_leg);
	fetch_transform(append_clone_number(right_leg), &og.right_leg);
	fetch_transform(append_clone_number(left_arm), &og.left_arm);
	fetch_transform(append_clone_number(right_arm), &og.right_arm);
	fetch_transform(append_clone_number(body), &og.body);
	fetch_transform(append_clone_number(head), &og.head);
	
	og.handle_walking(elapsed);
	//create_clone(glm::vec3(0, 0, 0), og);

	//update sound position:
	glm::vec3 center = glm::vec3(10.0f, 4.0f, 1.0f);
	float radius = 10.0f;

	//update listener position:
	glm::mat4 frame = current_camera->transform->make_local_to_world();

	//using the sound lock here because I want to update position and right-direction *simultaneously* for the audio code:
	Sound::lock();
	Sound::listener.set_position(frame[3]);
	Sound::listener.set_right(frame[0]);
	Sound::unlock();
}

void ObserveMode::draw(glm::uvec2 const &drawable_size) {
	//--- actual drawing ---
	glClearColor(0.85f, 0.85f, 0.90f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	const_cast< Scene::Camera * >(current_camera)->aspect = drawable_size.x / float(drawable_size.y);



	if (!in_intro || !in_ending) { scene->draw(*current_camera); }

	{ //help text overlay:
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawSprites draw(*trade_font_atlas, glm::vec2(0,0), glm::vec2(320, 200), drawable_size, DrawSprites::AlignPixelPerfect);

		if (in_intro || in_ending) {
			draw.draw(*background, ul);
			return;
		}

		if (play_set_up && play_set_up->stopped && !in_scope) {
			std::string help_text = "--- RIGHT CLICK TO SCOPE, LEFT CLICK TO SHOOT ---";
			glm::vec2 min, max;
			draw.get_text_extents(help_text, glm::vec2(0.0f, 0.0f), 1.0f, &min, &max);
			float x = std::round(160.0f - (0.5f * (max.x + min.x)));
			draw.draw_text(help_text, glm::vec2(x, 1.0f), 1.0f, glm::u8vec4(0x00, 0x00, 0x00, 0xff));
			draw.draw_text(help_text, glm::vec2(x, 2.0f), 1.0f, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
			return;
		}

		if (in_scope) { draw.draw(*scope, ul); }
	}
}
