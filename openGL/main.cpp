#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string.h>
#include<cmath>
#include<random>
#include<fstream>


using namespace std;

mt19937 gen(1943);

// window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, VBO1, shader;


// Vertex Shader
static const char* vShader = "                                         \n\
#version 330                                                           \n\
layout (location = 0) in vec3 pos;                                     \n\
layout (location = 1) in float state;                                  \n\
out float spin_color;						       \n\
void main()                                                            \n\
{                                                                      \n\
    gl_Position = vec4(pos, 1.0f);                                     \n\
    spin_color = state;						       \n\
                                                                       \n\
}                                                                      \n\
";

static const char* fShader = "               \n\
#version 330                                 \n\
in float spin_color;                         \n\
out vec4 colour;                             \n\
void main()                                  \n\
{   					     \n\
    if(spin_color == 1.0f){                  \n\
       colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n\
    }                                        \n\
    else if(spin_color == -1.0f){            \n\
       colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n\
    }					     \n\
}					     \n\
";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        cout << "Error Compiling the " << shaderType << " Shader!" << elog << endl;
    }

    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if(!shader)
    {
        cout << "Error creating shader program!" << endl;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar elog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        cout << "Error linking program!" << elog << endl;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        cout << "Error validating program!" << elog << endl;
    }

}


int main()
{
    // Initialize GLFW
    if(!glfwInit())
    {
        cout << "GLFW initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT,
                                               "Ising Model", NULL, NULL);
    if(!mainWindow)
    {
        cout << "GLFW window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }


    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        cout << "GLEW initialization failed!" << endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    ifstream in;
    in.open("../data/indices.dat");

    ifstream sp;
    sp.open("../data/init_state.dat");

    int Nspins{0};
    in >> Nspins;
    int Nspins2 = Nspins * Nspins;

    float x_start = -0.5f;
    float y_start =  0.5f;
    float dxy = 0.05f;
    GLfloat lattice[Nspins2][3]; //N * N spin lattice, every spin has 3 coords

    //we start from the upper left corner of the lattice and end in the lower right corner
    for(int i = 0 ; i < Nspins ; ++i)
    {
	for(int j = 0 ; j < Nspins ; ++j)
	{

		int index = Nspins * i + j; //from 2d lattice to 1d list (first dim of the array)

		lattice[index][0] = x_start + dxy * j; //x coord
		lattice[index][1] = y_start - dxy * i; //y coord
		lattice[index][2] = 0.0f;	     //z coord, always zero

	}

    }


    uniform_real_distribution<float> rand;

    GLfloat colors[Nspins2];

    int read_state;

    for(int i = 0 ; i < Nspins2 ; ++i)
    {
	sp >> read_state;
	colors[i] = read_state;

    }


    vector<int> sim_indices;
    int s{0};
    while(in >> s)
    {
	sim_indices.push_back(s);
    }

    int id{0};
    int changes = sim_indices.size();
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lattice), lattice, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);


    CompileShaders();

    // Loop until window closed
    while(!glfwWindowShouldClose(mainWindow))
    {
        // Get and Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

	glGenBuffers(1, &VBO1);
    	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    	glEnableVertexAttribArray(1);

    	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPointSize(20.0);
        glDrawArrays(GL_POINTS, 0, Nspins2);
        glBindVertexArray(0);


         //change the state of a randomly selected spin
	if(id < changes)
	{
	    int index = sim_indices[id];
	    colors[index] = - colors[index];

	    id++;
	}

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    in.close();
    sp.close();

    return 0;
}



