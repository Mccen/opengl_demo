//
tex[0] = loadTexture("./resources/imgs/earth.png");
tex[stone] = loadTexture("./resources/imgs/stone.png");

program[lightProgram] = fileloader("./resources/shaders/lightv.vs", "./resources/shaders/lightf.fs");
program[mainProgram] = fileloader("./resources/shaders/mainv.vs", "./resources/shaders/mainf.fs");
program[worldProgram] = fileloader("./resources/shaders/worldLight.vs", "./resources/shaders/worldLight.fs");

obj[Cube] = objLoader("./resources/models/block.obj");
obj[Sphere] = objLoader("./resources/models/sphere.obj");

Light(light[0],
      glm::vec4(1.0f, 1.0f, 1.0f, 0.2f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(5.0f, 4.0f, 5.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.1f, 0.09f, 0.005f));

Material(material[0], glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
