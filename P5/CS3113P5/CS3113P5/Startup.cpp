#include "Startup.h"



void Startup::Initialize() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

}

void Startup::Update(float deltaTime) {
	//Nothing to do here -- its the main menu!
}

void Startup::Render(ShaderProgram* program) {
	Util::DrawText(program,state.font, "Spingle's Greatest", 0.5, 0.001, glm::vec3(-4.2,2,0));
	Util::DrawText(program, state.font, "Adventure", 0.5, 0.001, glm::vec3(-1.85, 1.3, 0));
	Util::DrawText(program, state.font, "Press Enter to begin!", 0.25, 0.001, glm::vec3(-2.45, -1.3, 0));
}
