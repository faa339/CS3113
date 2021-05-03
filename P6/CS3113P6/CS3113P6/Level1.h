#include "Scene.h"

class Level1 : public Scene {
public:
	GLuint backGround;
	glm::mat4 bgModelMatrix;

	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void RenderBG(ShaderProgram* program);
	void Reset();
};