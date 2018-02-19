#include "shaderload.h"
#include "error.h"
#include "defines.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "lib/sdl.h"

using namespace std;


std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        stringstream ss;
        ss << "Cannot load shader " << filePath << ". File not found";
        error(ss.str());
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint shaderLoad(const char *vertex_path, const char *fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = readFile(string(string(LOC_ROOT)+string(vertex_path)).c_str());
    std::string fragShaderStr = readFile(string(string(LOC_ROOT)+string(fragment_path)).c_str());

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector <char> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    if(result==GL_FALSE){
        stringstream ss;
        ss<<"Cannot compile vertex shader ["<<vertex_path<<"]: "<<endl<<&vertShaderError[0];
        error(ss.str());
    }

    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector <char> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    if(result==GL_FALSE){
        stringstream ss;
        ss<<"Cannot compile fragment shader ["<<fragment_path<<"]: "<<endl<<&fragShaderError[0];
        error(ss.str());
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    if(result==GL_FALSE){
        stringstream ss;
        ss<<"Cannot link shader ["<<fragment_path<<"]["<<vertex_path<<"]: "<<endl<<&programError[0];
        error(ss.str());
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}


/*#ifdef _WIN32
#define shaderWIN
#endif // _WIN32

#ifdef __linux__
#define shaderLINUX
#endif // __linux__

#ifdef shaderWIN
#warning System ładowania shaderów naprawić!

GLuint shaderLoad(const char * vsh_file,const char * fsh_file){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(string(string(LOC_ROOT)+string(vsh_file)).c_str(), std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vsh_file);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(string(string(LOC_ROOT)+string(fsh_file)).c_str(), std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vsh_file);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fsh_file);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

#endif

#ifdef shaderLINUX
GLuint shaderLoad(const char * vsh_file,const char * fsh_file){

	string loc_vsh(string(LOC_ROOT)+string(vsh_file));
	string loc_fsh(string(LOC_ROOT)+string(fsh_file));
	int rw_vsh_length, rw_fsh_length;

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	{//sizes
	    SDL_RWops * rw_vsh_size = SDL_RWFromFile(loc_vsh.c_str(), "r");
        if(rw_vsh_size==NULL){
            error("Error loading vertex shader: File not found");
            return 0;
        }
        rw_vsh_length = SDL_RWseek(rw_vsh_size, 0, RW_SEEK_END);

	    SDL_RWops * rw_fsh_size = SDL_RWFromFile(loc_fsh.c_str(), "r");
        if(rw_fsh_size==NULL){
            error("Error loading fragment shader: File not found");
            return 0;
        }
        rw_fsh_length = SDL_RWseek(rw_fsh_size, 0, RW_SEEK_END);

        SDL_RWclose(rw_fsh_size);
        SDL_RWclose(rw_vsh_size);
	}

	SDL_RWops * rw_vsh = SDL_RWFromFile(loc_vsh.c_str(), "rb");
	SDL_RWops * rw_fsh = SDL_RWFromFile(loc_fsh.c_str(), "rb");

	char * vshcode = new char[rw_vsh_length];
	std::string VertexShaderCode;
	{
		SDL_RWread(rw_vsh, vshcode, rw_vsh_length, rw_vsh_length);
		VertexShaderCode = string(vshcode,rw_vsh_length);
		VertexShaderCode.append(1, '\0');
	}
	char * fshcode = new char[rw_fsh_length];
	std::string FragmentShaderCode;
	{
		SDL_RWread(rw_fsh, fshcode, rw_fsh_length, rw_fsh_length);
		FragmentShaderCode = string(fshcode,rw_fsh_length);
		FragmentShaderCode.append(1, '\0');
	}


	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 8){
		char errormsg[InfoLogLength+1];
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &errormsg[0]);
		error(string(string("Error loading vertex shader [")+string(vsh_file)+string("]: ")+string(errormsg)));
	}

	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 8 ){
		char errormsg[InfoLogLength+1];
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &errormsg[0]);
		error(string(string("Error loading fragment shader [")+string(fsh_file)+string("]: ")+string(errormsg)));
	}

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	SDL_RWclose(rw_vsh);
	SDL_RWclose(rw_fsh);

	//delete[]vshcode;
	//delete[]fshcode;

	return ProgramID;
}

#endif
*/
