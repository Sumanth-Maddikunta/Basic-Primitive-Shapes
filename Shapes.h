#pragma once
#include<GL/glew.h>
#include<vector>
#include<glm/glm.hpp>

const int SCR_WIDTH = 800, SCR_HEIGHT = 600;

class Shape
{
protected:
	unsigned int VAO, VBO;
	
public:
	Shape()
	{
		//std::cout << "In shape COnstuctor\n";
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		
	}
	~Shape()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	virtual void draw() = 0;
	virtual void SetOriginPosition(glm::vec2 origin) = 0;
	virtual void SetOtherPosition(glm::vec2 _point) = 0;
	virtual void SetBufferData() = 0;
	virtual glm::vec2* GetPoints() = 0;
	//virtual void SetNewPoint(glm::vec2 _point) = 0;
	
};

class Circle : public Shape
{
	glm::vec2 positions[2];
	std::vector<glm::vec2> vertices;
	float angleDiff;
public:
	Circle()
	{
		positions[0] = positions[1] = glm::vec2(0.0f, 0.0f);
		angleDiff = 10.0f;
	}
	~Circle()
	{

	}

	void SetOriginPosition(glm::vec2 origin)
	{
		positions[0] = origin;
		//positions[1] = circumfernce_point;
	}

	void SetOtherPosition(glm::vec2 _point)
	{
		//positions[0] = centre;
		positions[1] = _point;
	}

	
	void SetAngleDiff(float diff)
	{
		angleDiff = diff;
	}

	void SetBufferData()
	{
		vertices.clear();
		float radius = glm::distance(positions[0], positions[1]);
		glm::vec2 point = positions[0];
		//vertices.push_back(point);
		for (float i = 0.0f; i < 360.0f; i += angleDiff)
		{
			point.x = positions[0].x + cos(glm::radians(i))*radius;
			point.y = positions[0].y + sin(glm::radians(i))*radius * SCR_WIDTH/(float)SCR_HEIGHT;

			vertices.push_back(point);
		}
		vertices.push_back(positions[0] + glm::vec2(radius, 0.0f));
	}

	void draw()
	{
		if (positions[0] != positions[1])
		{
			SetBufferData();

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			
			{
				glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else
		{
			//std::cout << "Center is Same as Circle Point";
		}
	}

	glm::vec2* GetPoints()
	{
		glm::vec2 arr[2];
		arr[0] = positions[0];
		arr[1] = positions[1];
		return arr;
	}
};

class Line : public Shape
{
	glm::vec2 positions[2];
	std::vector<glm::vec2> vertices;

public:
	Line()
	{
		positions[0] = positions[1] = glm::vec2(0.0f, 0.0f);
	}
	~Line()
	{

	}
	void SetOriginPosition(glm::vec2 origin)
	{
		positions[0] = origin;
	}
	void SetOtherPosition(glm::vec2 _point)
	{
		positions[1] = _point;
	}
	
	void SetBufferData()
	{
		vertices.clear();
		vertices.push_back(positions[0]);
		vertices.push_back(positions[1]);
	}
	void draw()
	{
		if (positions[0] != positions[1])
		{
			SetBufferData();
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glDrawArrays(GL_LINES, 0, vertices.size());

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	glm::vec2* GetPoints()
	{
		glm::vec2 arr[2];
		arr[0] = positions[0];
		arr[1] = positions[1];
		return arr;
	}
};

class Rectangle : public Shape
{
	glm::vec2 positions[2];
	std::vector<glm::vec2> vertices;

public:
	Rectangle()
	{
		positions[0] = positions[1] = glm::vec2(0.0f, 0.0f);
	}
	~Rectangle()
	{

	}
	void SetOriginPosition(glm::vec2 origin)
	{
		positions[0] = origin;
	}
	void SetOtherPosition(glm::vec2 _point)
	{
		positions[1] = _point;
	}
	
	void SetBufferData()
	{
		vertices.clear();
		float xdiff = positions[1].x - positions[0].x;
		float ydiff = positions[1].y - positions[0].y;

		glm::vec2 point = glm::vec2(0.0f,0.0f);

		point = positions[0];
		vertices.push_back(point);

		point = glm::vec2(positions[0].x, positions[1].y);
		vertices.push_back(point);

		point = positions[1];
		vertices.push_back(point);

		point = glm::vec2(positions[1].x, positions[0].y);
		vertices.push_back(point);
	}
	void draw()
	{
		if (positions[0] != positions[1])
		{
			SetBufferData();
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			
				glDrawArrays(GL_LINE_LOOP, 0, vertices.size());


			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	glm::vec2* GetPoints()
	{
		glm::vec2 arr[4];
		arr[0] = vertices[0];
		arr[1] = vertices[1];
		arr[2] = vertices[2];
		arr[3] = vertices[3];
		return arr;
	}
};

class Polygon : public Shape
{
	std::vector<glm::vec2> points;

	public:
		Polygon()
		{

		}
		~Polygon()
		{

		}
		void SetOriginPosition(glm::vec2 origin)
		{
			if(origin!=glm::vec2(0.0f,0.0f))
			points.push_back(origin);
			//std::cout << origin.x << origin.y << "\n";
		}
		void SetOtherPosition(glm::vec2 _point)
		{
			if (_point != glm::vec2(0.0f, 0.0f))
				points.push_back(_point);
			//std::cout << _point.x << _point.y << "\n";

		}

		void SetBufferData()
		{
			
		}
		void draw()
		{
			SetBufferData();
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			/*if (isFill)
				glDrawArrays(GL_TRIANGLE_FAN, 2, points.size());
			else*/
				glDrawArrays(GL_LINE_LOOP, 0, points.size());


			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glm::vec2* GetPoints()
		{
			glm::vec2 arr[4];
			/*arr[0] = vertices[0];
			arr[1] = vertices[1];
			arr[2] = vertices[2];
			arr[3] = vertices[3];
			*/return arr;
		}
};
