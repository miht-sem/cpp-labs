
struct Small_Cube
{
	unsigned int color[6]; // [верх, низ, впереди, сзади, лево, право]
	double size;
public:
	Small_Cube(): size(0)
	{
		for (int i = 0; i < 6; ++i) color[i] = 0;
	}

	Small_Cube(const Small_Cube &cube) : size(cube.size)
	{
		for (int i = 0; i < 6; ++i)	color[i] = cube.color[i];	
	}

	Small_Cube &operator=(const Small_Cube &cube) = default;
	
	~Small_Cube() = default;

	void rotateX()
	{
		unsigned int tmp = color[0];
		color[0] = color[4];
		color[4] = color[1];
		color[1] = color[5];
		color[5] = tmp;
	}

	void rotateY()
	{
		unsigned int tmp = color[2];
		color[2] = color[1];
		color[1] = color[3];
		color[3] = color[0];
		color[0] = tmp;
	}

	void rotateZ()
	{
		unsigned int tmp = color[5];
		color[5] = color[3];
		color[3] = color[4];
		color[4] = color[2];
		color[2] = tmp;
	}

	void draw()
	{
		glPushMatrix();
		glBegin(GL_QUADS);

		// верх
		glColor3ubv(at(0));
		glNormal3f(0, 0, 1);
		glVertex3f(size, size, size);
		glVertex3f(0, size, size);
		glVertex3f(0, 0, size);
		glVertex3f(size, 0, size);

		// низ
		glColor3ubv(at(1));
		glNormal3f(0, 0, -1);
		glVertex3f(size, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, size, 0);
		glVertex3f(size, size, 0);

		// спереди
		glColor3ubv(at(2));
		glNormal3f(0, -1, 0);
		glVertex3f(size, 0, size);
		glVertex3f(0, 0, size);
		glVertex3f(0, 0, 0);
		glVertex3f(size, 0, 0);

		// сзади
		glColor3ubv(at(3));
		glNormal3f(0, 1, 0);
		glVertex3f(size, size, 0);
		glVertex3f(0, size, 0);
		glVertex3f(0, size, size);
		glVertex3f(size, size, size);

		// слева
		glColor3ubv(at(4));
		glNormal3f(-1, 0, 0);
		glVertex3f(0, size, size);
		glVertex3f(0, size, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, size);

		// справа
		glColor3ubv(at(5));
		glNormal3f(1, 0, 0);
		glVertex3f(size, size, 0);
		glVertex3f(size, size, size);
		glVertex3f(size, 0, size);
		glVertex3f(size, 0, 0);

		glEnd();
		glPopMatrix();
	}

	// отрисовка куба со смещением (x, y, z)
	void draw(double x, double y, double z)
	{
		glPushMatrix();
		glTranslated(x, y, z);
		draw();
		glPopMatrix();
	}

	friend std::ostream &operator<<(std::ostream &out, const Small_Cube &cube)
	{
		for (int i = 0; i < 6; ++i) out << cube.color[i] << " ";
		out << "\n";
		return out;
	}

	friend std::istream &operator>>(std::istream &in, Small_Cube &cube)
	{
		for (int i = 0; i < 6; ++i) in >> cube.color[i];
		return in;
	}

	void set_color(int side, int color)
	{
		this->color[side] = color;
	}

private:
	unsigned char _color[4];

	unsigned char *at(int i)
	{
		// разбиваем color[i] на 3 составляющих
		// например для 0xFF0000 RGB(FF, 0, 00) - красный цвет;
		_color[0] = color[i] >> 16;
		_color[1] = color[i] >> 8;
		_color[2] = color[i];
		return _color;
	}

};
