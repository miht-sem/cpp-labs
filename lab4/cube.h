#include "small_cube.h"

#define CUBE_SIZE 13

class Cube
{
private:
	// состоит из 27 маленьких кубиков
	Small_Cube m_cubes[3][3][3];
	// храним угол поворота каждой грани
	int m_rotates[7];
	// размер кубика-рубика
	double m_size;

public:
	// храним номер грани, которая в данный момент поварачивается, или -1 если ничего не поварачивается
	int current_turn;

	Cube(unsigned int *colors, double size = CUBE_SIZE) : m_size(size)
	{
		clear(colors);
	}

	Cube(const Cube &cube) : m_size(cube.m_size), current_turn(cube.current_turn)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					m_cubes[i][j][k] = cube.m_cubes[i][j][k];
				}
			}
		}

		for (int i = 0; i < 7; ++i)
		{
			m_rotates[i] = cube.m_rotates[i];
		}
	}

	~Cube() = default;

	Cube &operator=(const Cube &cube)
	{
		if (this == &cube)
			return *this;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					m_cubes[i][j][k] = cube.m_cubes[i][j][k];
				}
			}
		}

		for (int i = 0; i < 7; ++i)
		{
			m_rotates[i] = cube.m_rotates[i];
		}

		m_size = cube.m_size;
		current_turn = cube.current_turn;
		return *this;
	}

	void clear(unsigned int *color, double size_ = CUBE_SIZE)
	{
		for (int i = 0; i < 7; ++i)
			m_rotates[i] = 0;

		m_size = size_;
		current_turn = -1;

		int i, j, k;
		// верх
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				m_cubes[i][j][2].color[0] = color[0];

		// низ
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				m_cubes[i][j][0].color[1] = color[1];

		// спереди
		for (k = 0; k < 3; ++k)
			for (j = 0; j < 3; ++j)
				m_cubes[j][0][k].color[2] = color[2];

		// сзади
		for (k = 0; k < 3; ++k)
			for (j = 0; j < 3; ++j)
				m_cubes[j][2][k].color[3] = color[3];

		// слева
		for (i = 0; i < 3; ++i)
			for (k = 0; k < 3; ++k)
				m_cubes[0][k][i].color[4] = color[4];

		// справа
		for (i = 0; i < 3; ++i)
			for (k = 0; k < 3; ++k)
				m_cubes[2][k][i].color[5] = color[5];

		// устанавливаем размеры мелких деталей
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				for (k = 0; k < 3; ++k)
					m_cubes[i][j][k].size = (m_size / 3.0) * 0.95;
	}

	void draw()
	{
		const double K = 0.65;

		// рисуем корпус - это просто куб черного цвета, размер которого равен K*m_size
		glPushMatrix();
		glColor3f(0, 0, 0);
		glTranslatef(((1.0 - K) / 2) * m_size + K * m_size / 2, ((1.0 - K) / 2) * m_size + K * m_size / 2, ((1.0 - K) / 2) * m_size + K * m_size / 2);
		glutSolidCube(m_size * K);
		glPopMatrix();

		// ok[i][j][k] показывает, находится ли в состоянии покоя деталь с координатами (i, j, k)
		memset(ok, true, sizeof(ok));
		if (current_turn != -1)
		{
			glPushMatrix();
			int i, j, k;
			int ind_of_cur_side = (current_turn & 1) * 2;

			if (current_turn == 0 || current_turn == 1) // верх или низ
			{
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						ok[i][j][ind_of_cur_side] = false;

				glTranslated(m_size / 2, m_size / 2, 0);
				glRotatef(m_rotates[current_turn], 0, 0, 1);
				glTranslated(-m_size / 2, -m_size / 2, 0);

				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						m_cubes[i][j][ind_of_cur_side].draw(m_size / 3 * i, m_size / 3 * j, m_size / 3 * ind_of_cur_side);
			}
			else if (current_turn == 2 || current_turn == 3) // лево или право
			{
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						ok[i][ind_of_cur_side][k] = false;

				glTranslated(m_size / 2, 0, m_size / 2);
				glRotatef(m_rotates[current_turn], 0, 1, 0);
				glTranslated(-m_size / 2, 0, -m_size / 2);

				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						m_cubes[i][ind_of_cur_side][k].draw(m_size / 3 * i, m_size / 3 * ind_of_cur_side, m_size / 3 * k);
			}
			else if (current_turn == 4 || current_turn == 5) // перед или зад
			{
				for (j = 0; j < 3; ++j)
					for (k = 0; k < 3; ++k)
						ok[ind_of_cur_side][j][k] = false;

				glTranslated(0, m_size / 2, m_size / 2);
				glRotatef(m_rotates[current_turn], 1, 0, 0);
				glTranslated(0, -m_size / 2, -m_size / 2);

				for (j = 0; j < 3; ++j)
					for (k = 0; k < 3; ++k)
						m_cubes[ind_of_cur_side][j][k].draw(m_size / 3 * ind_of_cur_side, m_size / 3 * j, m_size / 3 * k);
			}
			else if (current_turn == 6) // середина сверху
			{
				j = 1;
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						ok[i][j][k] = false;

				glTranslated(m_size / 2, 0, m_size / 2);
				glRotatef(m_rotates[current_turn], 0, 1, 0);
				glTranslated(-m_size / 2, 0, -m_size / 2);
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						m_cubes[i][j][k].draw(m_size / 3 * i, m_size / 3 * j, m_size / 3 * k);
			}
			else if (current_turn == 7) // середина сбоку
			{
				k = 1;
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						ok[i][j][k] = false;

				glTranslated(m_size / 2, m_size / 2, 0);
				glRotatef(m_rotates[current_turn], 0, 0, 1);
				glTranslated(-m_size / 2, -m_size / 2, 0);

				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						m_cubes[i][j][k].draw(m_size / 3 * i, m_size / 3 * j, m_size / 3 * k);
			}
			glPopMatrix();
		}

		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				for (int k = 0; k < 3; ++k)
					if (ok[i][j][k])
						// теперь рисуем те детали, которые не поварачивались выше,
						// они отмечены ok[i][j][k] = true
						m_cubes[i][j][k].draw(m_size / 3 * i, m_size / 3 * j, m_size / 3 * k);
	}

	int calculate_incorrect_colors()
	{
		int total_amount = 0;
		int i, j, k;

		// верх
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				if (m_cubes[1][1][2].color[0] != m_cubes[i][j][2].color[0])
					total_amount++;

		// низ
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				if (m_cubes[1][1][0].color[1] != m_cubes[i][j][0].color[1])
					total_amount++;

		// перед
		for (k = 0; k < 3; ++k)
			for (j = 0; j < 3; ++j)
				if (m_cubes[j][0][k].color[2] != m_cubes[1][0][1].color[2])
					total_amount++;

		// ЗАД
		for (k = 0; k < 3; ++k)
			for (j = 0; j < 3; ++j)
				if (m_cubes[1][2][1].color[3] != m_cubes[j][2][k].color[3])
					total_amount++;

		// лево
		for (i = 0; i < 3; ++i)
			for (k = 0; k < 3; ++k)
				if (m_cubes[0][1][1].color[4] != m_cubes[0][k][i].color[4])
					total_amount++;

		// право
		for (i = 0; i < 3; ++i)
			for (k = 0; k < 3; ++k)
				if (m_cubes[2][1][1].color[5] != m_cubes[2][k][i].color[5])
					total_amount++;

		return total_amount;
	}

	// крутит грань под номером ind на угол angle (в градусах)
	void rotate(int ind, int direction = 1, int angle = 9)
	{
		// мы пытаемся покрутить грань с номером ind
		// значит нужно проверить что другая грань уже не крутится
		if (current_turn == -1 || current_turn == ind)
		{
			// обновляем поворот
			m_rotates[ind] += angle * direction;

			if (abs(m_rotates[ind]) < 90)
			{
				current_turn = ind;
			}
			else
			{
				// если угол стал кратным 90, то поварачиваем на массиве
				rotate90(ind, (m_rotates[ind] > 0) ^ (current_turn == 2 || current_turn == 3 || current_turn == 6));
				m_rotates[ind] = 0;
				current_turn = -1;
			}
		}
	}

	int rotate(std::string command, int angle = 9)
	{
		switch (command[0])
		{
		case 'f':
			current_turn = 2;
			switch (command[1])
			{
			case '1':
				rotate(2, 1, angle);
				return 1;
			case '\'':
				rotate(2, -1, angle);
				return -1;
			default:
				break;
			}
			break;

		case 'd':
			current_turn = 1;
			switch (command[1])
			{
			case '1':
				rotate(1, -1, angle);
				return -1;
			case '\'':
				rotate(1, 1, angle);
				return 1;
			default:
				break;
			}
			break;
		case 'u':
			current_turn = 0;
			switch (command[1])
			{
			case '1':
				rotate(0, 1, angle);
				return 1;
			case '\'':
				rotate(0, -1, angle);
				return -1;
			default:
				break;
			}
			break;
		case 'r':
			current_turn = 4;
			switch (command[1])
			{
			case '1':
				rotate(4, 1, angle);
				return 1;
			case '\'':
				rotate(4, -1, angle);
				return -1;
			default:
				break;
			}
			break;
		case 'b':
			current_turn = 3;
			switch (command[1])
			{
			case '1':
				rotate(3, -1, angle);
				return -1;
			case '\'':
				rotate(3, 1, angle);
				return 1;
			default:
				break;
			}
			break;
		case 'l':
			current_turn = 5;
			switch (command[1])
			{
			case '1':
				rotate(5, -1, angle);
				return -1;
			case '\'':
				rotate(5, 1, angle);
				return 1;
			default:
				break;
			}
			break;
		case 'm':
			current_turn = 6;
			switch (command[1])
			{
			case '1':
				rotate(6, 1, angle);
				return 1;
			case '\'':
				rotate(6, -1, angle);
				return -1;
			default:
				break;
			}
			break;
		case 'e':
			current_turn = 7;
			switch (command[1])
			{
			case '\'':
				rotate(7, -1, angle);
				break;
			default:
				rotate(7, 1, angle);
				break;
			}
			break;
		default:
			break;
		}
		return 0;
	}

	friend std::ostream &operator<<(std::ostream &out, const Cube &cube)
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				for (int k = 0; k < 3; ++k)
					out << cube.m_cubes[i][j][k];
		return out;
	}

	friend std::istream &operator>>(std::istream &in, Cube &cube)
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				for (int k = 0; k < 3; ++k)
					in >> cube.m_cubes[i][j][k];
		return in;
	}

private:
	int _angle[4];
	bool ok[4][4][4];
	Small_Cube tmp[4][4];

	void rotate90(int ind, int sign)
	{
		int rotations = sign ? 3 : 1;
		int i, j, k;
		Small_Cube tmp[4][4];
		int ind_of_rotating_side = (ind & 1) * 2;

		while (rotations--)
		{
			if (ind == 0 || ind == 1) // верх или низ
			{
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						tmp[j][2 - i] = m_cubes[i][j][ind_of_rotating_side];
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						tmp[i][j].rotateZ(), m_cubes[i][j][ind_of_rotating_side] = tmp[i][j];
			}
			else if (ind == 2 || ind == 3) // лево или право
			{
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						tmp[k][2 - i] = m_cubes[i][ind_of_rotating_side][k];
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						tmp[i][k].rotateX(), m_cubes[i][ind_of_rotating_side][k] = tmp[i][k];
			}
			else if (ind == 4 || ind == 5) // перед или ЗАД
			{
				for (j = 0; j < 3; ++j)
					for (k = 0; k < 3; ++k)
						tmp[k][2 - j] = m_cubes[ind_of_rotating_side][j][k];
				for (j = 0; j < 3; ++j)
					for (k = 0; k < 3; ++k)
						tmp[j][k].rotateY(), m_cubes[ind_of_rotating_side][j][k] = tmp[j][k];
			}
			else if (ind == 6)
			{
				// середина сверху
				j = 1;
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						tmp[k][2 - i] = m_cubes[i][j][k];
				for (i = 0; i < 3; ++i)
					for (k = 0; k < 3; ++k)
						tmp[i][k].rotateX(), m_cubes[i][j][k] = tmp[i][k];
			}
			else if (ind == 7)
			{
				// cередина сбоку
				k = 1;
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						tmp[j][2 - i] = m_cubes[i][j][k];
				for (i = 0; i < 3; ++i)
					for (j = 0; j < 3; ++j)
						tmp[i][j].rotateZ(), m_cubes[i][j][k] = tmp[i][j];
			}
		}
	}
};
