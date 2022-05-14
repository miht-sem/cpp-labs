#pragma once
#include <vector>
#include <map>
#include <algorithm>


#define number_of_formulas 19

typedef std::pair<Cube, std::string> cube_generation;

const std::string possible_rotates[12]{
	"u1", "u'", "d1", "d'", "f1", "f'",
	"b1", "b'", "r1", "r'", "l1", "l'"};

std::string get_random_rotation()
{
	return possible_rotates[rand() % 18];
}

class Genetic_Algorithm
{
private:
	const int population_size = 500;
	const int elitism_num = 50;
	const int max_generations = 300;
	const int max_moves = 10;
	const int min_moves = 1;
	const int max_formulas_moves = 3;

	Cube m_cube;

	const std::string formulas[number_of_formulas]
	{
		"f' l' b' r' u' r1 u' b1 l1 f1 r1 u1 r' u1",
		"f1 r1 b1 l1 u1 l' u1 b' r' f' l' u' l1 u'",
		"u1 u1 b1 u1 u1 b' r1 r1 f1 r' f' u1 u1 f' u1 u1 f1 r'",
		"u1 u1 r1 u1 u1 r' f1 f1 l1 f' l' u1 u1 l' u1 u1 l1 f'",
		"u' b1 b1 d1 d1 l' f1 f1 d1 d1 b1 b1 r' u'",
		"d' r' d1 r1 r1 u' r1 b1 b1 l1 u' l' b1 b1 u1 r1 r1",
		"d1 l1 d' l1 l1 u1 l' b1 b1 r' u1 r1 b1 b1 u1 l1 l1",
		"r' u1 l' u1 u1 r1 u' l1 r' u1 l' u1 u1 r1 u' l1 u'",
		"l1 u' r1 u1 u1 l' u1 r' l1 u' r1 u1 u1 l' u1 r' u1",
		"f' u1 b1 u' f1 u1 b' u'", "f1 u' b' u1 f' u' b1 u1",
		"l' u1 u1 l1 r' f1 f1 r1", "u1 r' u1 u1 r1 u1 r' u1 r1",
		"r' u1 u1 r1 l' b1 b1 l1", "r1 u' r1 u1 r1 u1 r1 u' r' u' r1 r1",
		"r1 r1 l1 l1 u1 r1 r1 l1 l1 u1 u1 r1 r1 l1 l1 u1 r1 r1 l1 l1",
		"u1 b1 b1 d1 d1 r1 f1 f1 d1 d1 b1 b1 l1 u1",
		"d1 l1 d' l1 l1 u1 l' b1 b1 r' u1 r1 b1 b1 u' l1 l1",
		"m1 m1 u1 m1 m1 u1 u1 m1 m1 u1 m1 m1"
	};

	std::map<char, char> x_rotate
	{
		{'u', 'b'},
		{'b', 'd'},
		{'d', 'f'},
		{'f', 'u'},
		{'e', 'd'},
		{'l', 'l'},
		{'r', 'r'},
		{'m', 'm'},
	};

	std::map<char, char> y_rotate
	{
		{'u', 'r'},
		{'r', 'd'},
		{'d', 'l'},
		{'l', 'u'},
		{'m', 'r'},
		{'f', 'f'},
		{'b', 'b'},
		{'e', 'e'},
	};

	std::map<char, char> z_rotate
	{
		{'f', 'l'},
		{'l', 'b'},
		{'b', 'r'},
		{'r', 'f'},
		{'e', 'f'},
		{'u', 'u'},
		{'d', 'd'},
		{'m', 'm'},
	};

public:
	Genetic_Algorithm(const Cube &cube) : m_cube(cube) {}

	std::pair<bool, std::string> algorithm()
	{
		if (m_cube.calculate_incorrect_colors() == 0)
			return std::make_pair(true, "Сube is already solved!");

		Cube best_gen(m_cube);
		auto generation = create_generation(best_gen);

		for (int i = 0; i < max_generations; ++i)
		{
			std::sort(generation.begin(), generation.end(), [](cube_generation lhs, cube_generation rhs)
					  { return lhs.first.calculate_incorrect_colors() < rhs.first.calculate_incorrect_colors(); });
			generation = evolution(generation);

			int best = generation[0].first.calculate_incorrect_colors();
			if (best == 0) return std::make_pair(true, generation[0].second);
		}
		return std::make_pair(false, generation[0].second);
	}

private:

	std::vector<cube_generation> create_generation(const Cube &cube)
	{
		std::vector<cube_generation> generation;
		generation.push_back(std::make_pair(cube, ""));
		for (int i = 1; i < population_size; ++i)
		{
			Cube instance(cube);
			std::string turns;
			turns = make_random_moves(instance);
			generation.push_back(std::make_pair(instance, turns));
		}
		return generation;
	}

	std::string make_random_moves(Cube &cube)
	{
		std::ostringstream res_moves;
		int moves_amount = rand() % max_moves + min_moves;

		while (moves_amount--)
		{
			auto move = get_random_rotation();
			res_moves << move << " ";
			cube.rotate(move, 90);
		}
		return res_moves.str();
	}

	std::vector<cube_generation> evolution(std::vector<cube_generation> &cubes)
	{
		for (int i = elitism_num; i < population_size; ++i)
		{
			cube_generation parent = cubes[rand() % elitism_num];
			Cube instance(parent.first);
			std::string turns;
			turns = random_formula_move(instance);
			cubes[i] = std::make_pair(instance, parent.second + turns);
		}
		return cubes;
	}

	std::string random_formula_move(Cube &cube)
	{
		auto moves = formulas[rand() % number_of_formulas];

		for (int i = 1; i < 4; ++i)
		{
			int rotates_amount = rand() % 4;
			while (rotates_amount--) moves = rotate_formula(moves, i);
		}

		std::istringstream commands(moves);
		std::string command;
		while (commands >> command) cube.rotate(command, 90);

		return moves;
	}

	std::string rotate_formula(std::string formula, int type_of_rotate)
	{
		std::istringstream commands(formula);
		std::ostringstream res_moves;
		std::string command;

		while (commands >> command)
		{
			switch (type_of_rotate)
			{
			case 1:
				command[0] = x_rotate[command[0]];
				break;
			case 2:
				command[0] = y_rotate[command[0]];
				break;
			case 3:
				command[0] = z_rotate[command[0]];
				break;
			default:
				break;
			}
			res_moves << command << " ";
		}

		return res_moves.str();
	}
};
