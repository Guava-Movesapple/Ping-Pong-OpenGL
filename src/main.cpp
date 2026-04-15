#include<iostream>
#include "Shader.h"
#include"glad/glad.h"
#include<GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void start() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framesiz_resize_callback1(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

int main(){
	
	start();

	GLFWwindow* pingpong = glfwCreateWindow(1500, 900, "Ping Pong", NULL, NULL);
	glfwMakeContextCurrent(pingpong);
	glfwSetFramebufferSizeCallback(pingpong, framesiz_resize_callback1);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	float vertices[] = {
	    //boardvertices     //ballvertices
		-0.015f,  0.25f,    -0.01f,  0.01f,
		 0.015f,  0.25f,     0.01f,  0.01f,
		 0.015f, -0.25f,     0.01f, -0.01f,
		-0.015f, -0.25f,    -0.01f, -0.01f
	};


	int element[] = {
		0, 1, 2,
		0, 3, 2
	};

	GLuint vbo, ebo, vao[2];
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(2, vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindVertexArray(vao[0]);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element), element, GL_STATIC_DRAW);

	glBindVertexArray(0);


	glBindVertexArray(vao[1]);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element), element, GL_STATIC_DRAW);

	glBindVertexArray(0);

	Shader shader("pingpong.vs", "pingpong.fs");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	bool gameStarted = false;

	glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.0f, 0.0f));
	glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.95f, 0.0f, 0.0f));
	glm::vec2 ballDirection(0.0f, 0.0f);
	float ballSpeed = 0.0001f;
	//ballDirection = glm::normalize(ballDirection);
	glm::mat4 ballModel(1.0f);

	float player1Position = 0, player2Position = 0;

	shader.Activate();

	int modelLoc = glGetUniformLocation(shader.ID, "model");


	while (!glfwWindowShouldClose(pingpong)) {


		if (glfwGetKey(pingpong, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(pingpong, true);
		}
		
		if (glfwGetKey(pingpong, GLFW_KEY_SPACE) == GLFW_PRESS && !gameStarted){
			gameStarted = true;
			ballDirection = glm::vec2(-1.0f, 0.0f);
		}
		else if (!gameStarted){
			goto render;
		}


		if (ballModel[3][1] >= 0.99f || ballModel[3][1] <= -0.99f) {
			ballDirection[1] *= -1;
			ballDirection = glm::normalize(ballDirection);
			ballModel = glm::translate(ballModel, glm::vec3(0.0f,ballDirection[1], 0.0f) * ballSpeed);
		}

		if ((ballModel[3][0] > 1 || ballModel[3][0] < -1) && gameStarted) {
			gameStarted = false;
			ballModel = glm::mat4(1.0f);
			ballDirection = glm::vec2(0.0f, 0.0f);
			model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.0f, 0.0f));
			player1Position = 0.0f;
			model2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.95f, 0.0f, 0.0f));
			player2Position = 0.0f;
			goto render;
		}
		
		if (ballModel[3][0] < -0.9f && ballModel[3][0] > -0.91f && (ballModel[3][1] > player1Position - vertices[1] && ballModel[3][1] < player1Position + vertices[1])) {
			ballDirection[0] *= -1;

			float ballDirectionY = 0.3f + (((ballModel[3][1] - player1Position) / 0.5f) * 3.0f);
			ballDirection[1] = ballDirectionY;

			ballDirection = glm::normalize(ballDirection);
			ballModel = glm::translate(ballModel, glm::vec3(ballDirection, 0.0f) * ballSpeed);
		}

		if (ballModel[3][0] > 0.9f && ballModel[3][0] < 0.91f && (ballModel[3][1] > player2Position - vertices[1] && ballModel[3][1] < player2Position + vertices[1])) {
			ballDirection[0] *= -1;

			float ballDirectionY = 0.3 + (((ballModel[3][1] - player2Position) / 0.5f) * 3);
			ballDirection[1] = ballDirectionY;
			
			ballDirection = glm::normalize(ballDirection);
			ballModel = glm::translate(ballModel, glm::vec3(ballDirection, 0.0f) * ballSpeed);
		}

		if (glfwGetKey(pingpong, GLFW_KEY_W) == GLFW_PRESS && player1Position < 0.74f) {
			player1Position += 0.0001f;
			model1 = glm::translate(model1, glm::vec3(0.0f, 0.0001f, 0.0f));
		}
		else if (glfwGetKey(pingpong, GLFW_KEY_S) == GLFW_PRESS && player1Position > -0.74f) {
			player1Position -= 0.0001f;
			model1 = glm::translate(model1, glm::vec3(0.0f, -0.0001f, 0.0f));
		}

		if (glfwGetKey(pingpong, GLFW_KEY_UP) == GLFW_PRESS && player2Position < 0.74f) {
			player2Position += 0.0001f;
			model2 = glm::translate(model2, glm::vec3(0.0f, 0.0001f, 0.0f));
		}
		else if (glfwGetKey(pingpong, GLFW_KEY_DOWN) == GLFW_PRESS && player2Position > -0.74f) {
			player2Position -= 0.0001f;
			model2 = glm::translate(model2, glm::vec3(0.0f, -0.0001f, 0.0f));
		}

		ballModel = glm::translate(ballModel, glm::vec3(ballDirection, 0.0f) * ballSpeed);

		render:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindVertexArray(vao[0]);

		//player1 board
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//player2 board
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glBindVertexArray(vao[1]);

		//ball
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ballModel));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		

		glfwSwapBuffers(pingpong);
		glfwPollEvents();
	}

	return 0;
}
