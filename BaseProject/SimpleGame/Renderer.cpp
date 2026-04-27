#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <vector>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_TriangleShader = CompileShaders("./Shaders/Triangle.vs", "./Shaders/Triangle.fs");
	m_BaseShader = CompileShaders("./Shaders/Base.vs", "./Shaders/Base.fs");

	m_RgbTexture = CreatePngTexture("./Textures/rgb.png", GL_NEAREST);
	m_NumsTexture = CreatePngTexture("./Textures/numbers.png", GL_NEAREST);
	for (int i = 0; i < 10; i++)
	{
		std::string path = "./Textures/" + std::to_string(i) + ".png";
		m_NumTexture[i] = CreatePngTexture((char*)path.c_str(), GL_NEAREST);
	}

	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float centerX = 0;
	float centerY = 0;
	float size = 0.1;
	float mass = 1;
	float vx = 1;
	float vy = 1;

	float triangle[]
		=
	{
		centerX - size / 2, centerY - size / 2, 0,	
		mass, vx, vy, // V0
		centerX + size / 2, centerY - size / 2, 0,	
		mass, vx, vy, // V1
		centerX + size / 2, centerY + size / 2, 0,	
		mass, vx, vy, // V2

		centerX - size / 2, centerY - size / 2, 0,	
		mass, vx, vy, // V3
		centerX + size / 2, centerY + size / 2, 0,	
		mass, vx, vy, // V4
		centerX - size / 2, centerY + size / 2, 0,	
		mass, vx, vy, // V5
	};

	glGenBuffers(1, &m_TriangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	float baseRect[] =
	{
		-1.f, -1.f, 0.f, 0, 1,
		 1.f, -1.f, 0.f, 1, 1,
		 1.f,  1.f, 0.f, 1, 0,
		-1.f, -1.f, 0.f, 0, 1,
		 1.f,  1.f, 0.f, 1, 0,
		-1.f,  1.f, 0.f, 0, 0,
	};

	glGenBuffers(1, &m_BaseVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_BaseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(baseRect), baseRect, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); 

	if (ShaderProgram == 0) { 
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawBaseRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	//Program select
	glUseProgram(m_BaseShader);

	int attribPosition = glGetAttribLocation(m_BaseShader, "a_Pos");
	int attribTexture = glGetAttribLocation(m_BaseShader, "a_Tex");
	int uRGBTexture = glGetUniformLocation(m_BaseShader, "u_RGBTex");
	glUniform1i(uRGBTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RgbTexture);

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribTexture);
	glBindBuffer(GL_ARRAY_BUFFER, m_BaseVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(attribTexture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

float gTime = 0;

void Renderer::DrawTriangle()
{
	gTime += 0.001;

	//Program select
	glUseProgram(m_TriangleShader);

	int u_Time = glGetUniformLocation(m_TriangleShader, "u_Time");
	glUniform1f(u_Time, gTime);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);

	glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
	//Load Png
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		std::cout << "PNG image loading failed:" << filePath << std::endl;
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, &image[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);
	return temp;
}

void Renderer::GenParticle(int count)
{
	m_ParticleCount = count;

	float size = 0.01f;
	float mass = 1.0f;

	// 모든 파티클이 공유하는 기본 사각형 위치 (6 vertices)
	float basePos[] = {
		-size / 2, -size / 2, 0,
		 size / 2, -size / 2, 0,
		 size / 2,  size / 2, 0,
		-size / 2, -size / 2, 0,
		 size / 2,  size / 2, 0,
		-size / 2,  size / 2, 0,
	};

	// 버텍스 1개 = x, y, z, mass, vx, vy, randomValue, randomValue1 = 8 floats
	std::vector<float> particleData;
	particleData.reserve(count * 6 * 9);

	for (int i = 0; i < count; i++)
	{
		float vx = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
		float vy = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
		float randValue = ((float)rand() / RAND_MAX) * 2 - 1;   // -1 ~ 1 랜덤 시작 시간
		float randValue1 = ((float)rand() / RAND_MAX) * 2 - 1;  // -1 ~ 1 랜덤 시작 시간
		float randValue2 = ((float)rand() / RAND_MAX) * 2;  // 0 ~ 2 랜덤 시작 시간

		for (int v = 0; v < 6; v++)
		{
			particleData.push_back(basePos[v * 3 + 0]);
			particleData.push_back(basePos[v * 3 + 1]);
			particleData.push_back(basePos[v * 3 + 2]);
			particleData.push_back(mass);
			particleData.push_back(vx);
			particleData.push_back(vy);
			particleData.push_back(randValue);
			particleData.push_back(randValue1);
			particleData.push_back(randValue2);
		}
	}

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, particleData.size() * sizeof(float), particleData.data(), GL_STATIC_DRAW);
}

void Renderer::DrawParticle()
{
	gTime += 0.002f;
	glUseProgram(m_TriangleShader);

	glUniform1f(glGetUniformLocation(m_TriangleShader, "u_Time"), gTime);

	int attribPosition = glGetAttribLocation(m_TriangleShader, "a_Position");
	int attribMass = glGetAttribLocation(m_TriangleShader, "a_Mass");
	int attribVel = glGetAttribLocation(m_TriangleShader, "a_Vel");
	int attribRandValue = glGetAttribLocation(m_TriangleShader, "a_randomValue");
	int attribRandValue1 = glGetAttribLocation(m_TriangleShader, "a_randomValue1");
	int attribRandValue2 = glGetAttribLocation(m_TriangleShader, "a_randomValue2");

	glEnableVertexAttribArray(attribPosition);
	glEnableVertexAttribArray(attribMass);
	glEnableVertexAttribArray(attribVel);
	glEnableVertexAttribArray(attribRandValue);
	glEnableVertexAttribArray(attribRandValue1);
	glEnableVertexAttribArray(attribRandValue2);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glVertexAttribPointer(attribMass, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(attribVel, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 4));
	glVertexAttribPointer(attribRandValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(attribRandValue1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 7));
	glVertexAttribPointer(attribRandValue2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (GLvoid*)(sizeof(float) * 8));

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCount * 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribMass);
	glDisableVertexAttribArray(attribVel);
	glDisableVertexAttribArray(attribRandValue);
	glDisableVertexAttribArray(attribRandValue1);
	glDisableVertexAttribArray(attribRandValue2);
}