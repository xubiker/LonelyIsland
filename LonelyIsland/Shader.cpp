#include "Shader.h"

void Shader::InitTexture()
{
	glGenTextures(3, texID);
}

void Shader::LoadTrueColorTexture(const char *path, GLenum program, int idx, const char *sampler)
{
	int tex_width, tex_height;
	unsigned char *tex_bits;	
	if((tex_bits = LoadTrueColorBMPFile(path, &tex_width, &tex_height)) == NULL)
		throw std::exception("Cannot open texture file");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0 + idx);
	glBindTexture(GL_TEXTURE_2D, texID[idx]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bits);
	// ”станавливаем параметры текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete[] tex_bits;
}

void Shader::PrintInfoLog(const char *prefix, GLenum obj)
{
	char log[10000];
	int log_len;
	glGetInfoLogARB(obj, sizeof(log)/sizeof(log[0]) - 1, &log_len, log);
	log[log_len] = 0;
	cout << prefix << ": " << log;
}

bool LoadFile(const char *fileName, bool binary, char **buffer, int *size)
{
	FILE* input;
	int  fileSize, readed;
	const char mode[] = {'r', binary ? 'b' : 't', '\0'};

	if ((input = fopen(fileName, mode)) == NULL)
		return false;
	fseek(input, 0, SEEK_END);
	fileSize = (int)ftell(input);
	rewind(input);
	if (fileSize == 0)
	{
		fclose(input);
		return false;
	}
	*buffer = new char[fileSize];
	readed = fread(*buffer, 1, fileSize, input);
	fclose(input);
	if (readed != fileSize)
	{
		delete[] *buffer;
		return false;
	}
	*size = fileSize;
	return true;
}

GLuint LoadShader(const char *vsName, const char *fsName)
{
	GLuint program, shader;
	char* shaderSource;
	int sourceLength;

	// слздадим шейдерную программу
	if ((program = glCreateProgram()) == 0)
		return 0;
	// если необходимо создать вершинный шейдер
	if (vsName)
	{
		// создадим вершинный шейдер
		if ((shader = glCreateShader(GL_VERTEX_SHADER)) == 0)
		{
			glDeleteProgram(program);
			return 0;
		}
		// загрузим вершинный шейдер
		if (!LoadFile(vsName, true, &shaderSource, &sourceLength))
		{
			glDeleteShader(shader);
			glDeleteProgram(program);
			return 0;
		}
		// добавим к коду вершинного шейдера параметры
		static const char vertexShaderDefines[] =
			"#version 330 core\n"\
			"#define VERT_POSITION 0\n"\
			"#define VERT_TEXCOORD 1\n"\
			"#define VERT_NORMAL 2\n"\
			"\n";

		const GLchar *source[2] = {(const GLchar*)vertexShaderDefines, (const GLchar*)shaderSource};
		const GLint  length[2] = {sizeof(vertexShaderDefines) - 1, sourceLength};

		// зададим шейдеру исходный код и скомпилируем его
		glShaderSource(shader, 2, source, length);
		glCompileShader(shader);

		delete[] shaderSource;

		// присоеденим загруженные шейдеры к шейдерной программе
		glAttachShader(program, shader);

		// шейдер нам больше не нужен, пометим его на удаление
		// он будет удален вместе с шейдерной программой
		glDeleteShader(shader);
	}

	// если необходимо создать фрагментный шейдер
	if (fsName)
	{
		// создадим вершинный шейдер
		if ((shader = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
		{
			//LOG_ERROR("Creating fragment shader fail (%d)\n", glGetError());
			glDeleteProgram(program);
			return 0;
		}

		// загрузим фрагментный шейдер
		if (!LoadFile(fsName, true, &shaderSource, &sourceLength))
		{
			glDeleteShader(shader);
			glDeleteProgram(program);
			return 0;
		}

		// добавим к коду фрагментного шейдера параметры
		static const char fragmentShaderDefines[] =
			"#version 330 core\n"\
			"#define FRAG_OUTPUT0 0\n"\
			"\n";
		const GLchar *source[2] = {(const GLchar*)fragmentShaderDefines, (const GLchar*)shaderSource};
		const GLint  length[2] = {sizeof(fragmentShaderDefines) - 1, sourceLength};

		// зададим шейдеру исходный код и скомпилируем его
		glShaderSource(shader, 2, source, length);
		glCompileShader(shader);

		delete[] shaderSource;

		// проверим статус компил€ции шейдера
		if (true)
		{
			glDeleteShader(shader);
			glDeleteProgram(program);
			return 0;
		}

		// присоеденим загруженные шейдеры к шейдерной программе
		glAttachShader(program, shader);

		// шейдер нам больше не нужен, пометим его на удаление
		// он будет удален вместе с шейдерной программой
		glDeleteShader(shader);
	}
	
	return program;
}

bool Shader::initShader (const char* shV, const char* shF)
{
	if(!GLEW_ARB_shading_language_100)
	{
		cout << "No GLSL 1.00 support" << endl;
		return false;
	}
 
	string vsh_src((istreambuf_iterator<char>(fstream(shV))), istreambuf_iterator<char>());
	string fsh_src((istreambuf_iterator<char>(fstream(shF))), istreambuf_iterator<char>());

	program = glCreateProgramObjectARB();	
	GLenum vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	GLenum fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	// Load Shader Sources
	const char *src = vsh_src.c_str();
	glShaderSourceARB(vertex_shader, 1, &src, NULL);
	src = fsh_src.c_str(); 
	glShaderSourceARB(fragment_shader, 1, &src, NULL);

	// Compile The Shaders
	glCompileShaderARB(vertex_shader);
	PrintInfoLog("compiling vertex shader", vertex_shader);
	glCompileShaderARB(fragment_shader);
	PrintInfoLog("compiling fragment shader", fragment_shader);

	// Attach The Shader Objects To The Program Object
	glAttachObjectARB(program, vertex_shader);
	glAttachObjectARB(program, fragment_shader);
	
	// Link The Program Object
	glLinkProgramARB(program);

	// Use The Program Object Instead Of Fixed Function OpenGL
	glUseProgramObjectARB(program);

	InitTexture();
	LoadTrueColorTexture("platform.bmp", program, 2, "diffuseMap");
	LoadTrueColorTexture("hero.bmp", program, 1, "diffuseMap");
	LoadTrueColorTexture("MountainGrass.bmp", program, 0, "diffuseMap");

	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	return true;
}

void Shader::setupTexture(GLint id, const GLchar *name,GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, name), id);
}
