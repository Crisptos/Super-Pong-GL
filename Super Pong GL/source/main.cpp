#include <stdio.h>
#include "glad/glad.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "entity.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

const float PADDLE_BOUNDS = 5.75f;
const float PADDLE_SPEED = 8.0f;

// Windowing
SDL_Window* m_Window;
SDL_GLContext m_glContext;
bool m_isRunning = false;

// Entities
Entity p1Paddle({0.0f, 0.0f, 2.0f});
Entity p2Paddle({ 0.0f, 0.0f, -18.0f });
Entity ball({0.0f, 0.0f, -6.0f});
Entity arena({ 0.0f, 0.0f, -8.0f });

Shader def;

float delta_time = 0;
float last_time = 0;

unsigned int texture;

// _________________________ Runtime _________________________
void Input()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT: { m_isRunning = false; break; }
		}
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		p1Paddle.position.x -= PADDLE_SPEED * delta_time;
	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		p1Paddle.position.x += PADDLE_SPEED * delta_time;
	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		p2Paddle.position.x -= PADDLE_SPEED * delta_time;
	}

	if (currentKeyStates[SDL_SCANCODE_D])
	{
		p2Paddle.position.x += PADDLE_SPEED * delta_time;
	}

	if (currentKeyStates[SDL_SCANCODE_I])
	{
		printf("P1 Pos (X: %.2f, Y: %.2f, Z: %.2f\n", p1Paddle.position.x, p1Paddle.position.y, p1Paddle.position.z);
		printf("P2 Pos (X: %.2f, Y: %.2f, Z: %.2f\n", p2Paddle.position.x, p2Paddle.position.y, p2Paddle.position.z);
		SDL_Delay(100);
	}

	if (p1Paddle.position.x > PADDLE_BOUNDS) p1Paddle.position.x = PADDLE_BOUNDS;
	if (p1Paddle.position.x < -PADDLE_BOUNDS) p1Paddle.position.x = -PADDLE_BOUNDS;

	if (p2Paddle.position.x > PADDLE_BOUNDS) p2Paddle.position.x = PADDLE_BOUNDS;
	if (p2Paddle.position.x < -PADDLE_BOUNDS) p2Paddle.position.x = -PADDLE_BOUNDS;
}

void UpdateBall()
{
	ball.position += ball.velocity * delta_time;

	// Collision Resolution
	if (ball.position.x > 9.0f) ball.velocity.x *= -1;
	if (ball.position.x < -9.0f) ball.velocity.x *= -1;

	// Reset Ball
	if (ball.position.z > 2.0f || ball.position.z < -18.0f) ball.position = { 0.0f, 0.0f, -6.0f };


}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float current_time = SDL_GetTicks();
	delta_time = (current_time - last_time)/1000.0f;
	last_time = current_time;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)(WINDOW_WIDTH) / (float)(WINDOW_HEIGHT), 0.1f, 100.0f);

	view = glm::rotate(view, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, -16.0f, -5.0f));

	glBindTexture(GL_TEXTURE_2D, texture);
	p1Paddle.UpdateEntity();
	p2Paddle.UpdateEntity();
	arena.UpdateEntity();
	ball.UpdateEntity();

	def.setUniMat4("view", view);
	def.setUniMat4("projection", projection);

	def.setUniMat4("model", arena.m_transform);
	arena.DrawEntity();
	
	def.setUniMat4("model", p1Paddle.m_transform);
	p1Paddle.DrawEntity();

	def.setUniMat4("model", p2Paddle.m_transform);
	p2Paddle.DrawEntity();

	def.setUniMat4("model", ball.m_transform);
	ball.DrawEntity();

	SDL_GL_SwapWindow(m_Window);
}

// _________________________ Init and Quiting _________________________

void Destroy()
{
	printf("Shutting Down...\n");
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void LoadTexture()
{
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/UV-Grid.png", &width, &height, &nrChannels, 0);

	if (!data) { printf("Error loading texture!\n"); exit(1); }

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void SetupGL()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glClearColor((float)45 / 255, (float)45 / 255, (float)45 / 255, 1.0f);
	def.genShader();
	def.loadFile("resources/shaders/vertex.glsl", GL_VERTEX_SHADER);
	def.loadFile("resources/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	def.compileShader();
	def.runShader();

	p1Paddle.m_mesh.LoadMesh("resources/meshes/paddle.obj");
	p2Paddle.m_mesh.LoadMesh("resources/meshes/paddle.obj");
	ball.m_mesh.LoadMesh("resources/meshes/sphere.obj");
	arena.m_mesh.LoadMesh("resources/meshes/arena.obj");

	LoadTexture();
}

void Init()
{
	printf("Initializing...\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		const char* sdl_error = SDL_GetError();
		printf("Error initializing SDL2! %s\n", sdl_error);
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	m_Window = SDL_CreateWindow(
		"Super Pong GL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == NULL)
	{
		const char* sdl_error = SDL_GetError();
		printf("Error creating SDL2 window! %s\n", sdl_error);
		exit(1);
	}

	m_glContext = SDL_GL_CreateContext(m_Window);

	if (m_glContext == NULL)
	{
		const char* sdl_error = SDL_GetError();
		printf("Error initializing SDL2 OpenGL Context! %s\n", sdl_error);
		exit(1);
	}

	if (gladLoadGLLoader(SDL_GL_GetProcAddress) == NULL)
	{
		printf("Error initializing GLAD!\n");
		exit(1);
	}

	m_isRunning = true;

}

int main(int argc, char** argv)
{
	Init();
	SetupGL();
	ball.velocity.z = 6.0f;
	//ball.velocity.x = 4.0f;
	while (m_isRunning)
	{
		Input();
		UpdateBall();
		Draw();
	}

	Destroy();

	return 0;
}