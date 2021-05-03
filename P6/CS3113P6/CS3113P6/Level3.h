#include "Scene.h"

class Level3 : public Scene {
public:
	GLuint backGround;
	GLuint mapTexID;
	glm::mat4 bgModelMatrix;
	bool mapSwitch;
	
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void RenderBG(ShaderProgram* program);
	void Reset();
};