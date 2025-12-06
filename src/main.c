#include "../raylib/src/raylib.h"
#include "../raylib/src/raymath.h"
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int windowWidth = 1000;
int windowHeight = 500;

void Update() {
	//use GetFrameTime()
}

#define bgShaderCount 1
Shader bgShaders[bgShaderCount];
int currentBGShaderIndex = 0;
void DrawBackground() {
	
	BeginShaderMode(bgShaders[currentBGShaderIndex]);
	float totalTime = GetTime();
	SetShaderValue(bgShaders[currentBGShaderIndex], GetShaderLocation(bgShaders[currentBGShaderIndex], "totalTime"), &totalTime, SHADER_UNIFORM_FLOAT);
	DrawRectangle(0, 0, windowWidth, windowHeight, BLACK);
	EndShaderMode();
}

void Initialize() {
	InitWindow(windowWidth, windowHeight, "Template-4.0.0");
	for (int i = 0; i < bgShaderCount; i++)
		bgShaders[i] = LoadShader(0, TextFormat("%s%s", GetApplicationDirectory(), TextFormat("../resources/shaders/background%i.fs", i)));
}

void CloseProgram() {
   	CloseWindow();
}


int main()
{
	Initialize();

	while (!WindowShouldClose()) {

		Update();

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawBackground();

		EndDrawing();
	}	
	CloseProgram();
   	return 0;
}