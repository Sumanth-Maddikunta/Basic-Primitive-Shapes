#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include<iostream>
#include<vector>

#include "Shapes.h"

//const int SCR_WIDTH = 800, SCR_HEIGHT = 600;

enum Shape_Type
{
	LINE,RECTANGLE,CIRCLE,POLYGON
};
Shape_Type current = CIRCLE;



std::vector<Shape*> shapes;

Shape *C;


glm::vec2 points[2] = { glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f) };

//Circle Parameters
const float angleDiff = 10.0f;
float radius = 0.5f;

//


const char* vertexShaderSource =
"#version 330 core"
"\nlayout(location = 0)in vec2 aPos;"
"\nvoid main(){\n"
"gl_Position = vec4(aPos.x,aPos.y,0.0f,1.0f);}\0";


const char* fragmentShaderSource =
"#version 330 core"
"\nvoid main(){\n"
"gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);}\0";


bool firstClick = true;
bool firstUndo = true;
bool secondPoint = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

glm::vec2 ScreenToWorldPoint(glm::vec2 point);
Shape* ShapeFactory();
void menu();


int main()

{

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Paint Program", NULL, NULL);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Init Failed";

		glfwTerminate();

		exit(-1);

	}

	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);

	glCompileShader(vertexShader);


	int success;

	char infoLog[512];


	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);

		std::cout << "\nVertex Shader Compilation Failed\n" << infoLog;
	}


	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);

		std::cout << "\nFragmnet Shader Compilation Failed\n" << infoLog;
	}


	unsigned int shaderProg;

	shaderProg = glCreateProgram();

	glAttachShader(shaderProg, vertexShader);

	glAttachShader(shaderProg, fragmentShader);

	glLinkProgram(shaderProg);

	glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProg, 512, 0, infoLog);

		std::cout << "\nShader Program Linking Failed\n" << infoLog;
	}

	glDeleteShader(vertexShader);

	glDeleteShader(fragmentShader);

	glUseProgram(shaderProg);

	


	
	C = NULL;

	C = ShapeFactory();
	shapes.push_back(C);

	

	menu();

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(1.0f, 1.0f, 0.0, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);


		

		

		processInput(window);

		

		for (int i = 0; i < shapes.size(); i++)
			shapes[i]->draw();
		
		
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
		

	glfwTerminate();

	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);

}



void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwTerminate();

		exit(0);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		double xpos, ypos;
		
		firstClick = true;
		if (secondPoint)
		{
			glfwGetCursorPos(window, &xpos, &ypos);

			
			
			glm::vec2 Coord = glm::vec2(xpos, ypos);

			Coord = ScreenToWorldPoint(Coord);

			if(current!=POLYGON)
				C->SetOtherPosition(Coord); 


		}

	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos, ypos;
		if (firstClick)
		{
			glfwGetCursorPos(window, &xpos, &ypos);

			

			firstClick = false;

			glm::vec2 Coord = glm::vec2(xpos, ypos);

			Coord = ScreenToWorldPoint(Coord);

			if (secondPoint)
			{
				C->SetOtherPosition(Coord); 

			}
					 
			else
			{
				if (current != POLYGON)
				{
					C = ShapeFactory();
					shapes.push_back(C);

					

					glm::vec2 Coord = glm::vec2(xpos, ypos);

					Coord = ScreenToWorldPoint(Coord);

					C->SetOriginPosition(Coord);
					C->SetOtherPosition(Coord);
				}
				else
				{
					secondPoint = !secondPoint;

					glm::vec2 Coord = glm::vec2(xpos, ypos);

					Coord = ScreenToWorldPoint(Coord);

					C->SetOriginPosition(Coord);
					C->SetOtherPosition(Coord);
				}
				
			}
			secondPoint = !secondPoint;
		}

	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		secondPoint = false;
		firstClick = true;
		current = CIRCLE;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		secondPoint = false;
		firstClick = true;
		current = LINE;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		secondPoint = false;
		firstClick = true;
		current = RECTANGLE;
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		shapes.clear();
	}
	
}


glm::vec2 ScreenToWorldPoint(glm::vec2 point)
{
	glm::vec2 newPoint = glm::vec2(0.0f, 0.0f);
	newPoint.x = (point.x - SCR_WIDTH / 2.0) / (SCR_WIDTH / 2.0);

	newPoint.y = -(point.y - SCR_HEIGHT / 2.0) / (SCR_HEIGHT / 2.0);

	return newPoint;

}


Shape* ShapeFactory()
{
	if (current == LINE)
		return new Line();
	else if (current == RECTANGLE)
		return new Rectangle();
	else if (current == CIRCLE)
		return new Circle();
	
	else
		return new Line();
}


void menu()
{
	std::cout << "\nKey\tFunction"
		<< "\n1\tCircle"
		<< "\n2\tLine"
		<< "\n3\tRectangle"
		<<"\nSpace\tClear";
}

