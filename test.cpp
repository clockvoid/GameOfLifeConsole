#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

class callback {
	public:
		std::vector<std::vector<int>> operator()(std::vector<std::vector<int>> vector, int size);
};

std::vector<std::vector<int>> callback::operator()(std::vector<std::vector<int>> vector, int size) {
	std::vector<std::vector<int>> vec;
	for (auto i = 0; i < size; i++) {
		std::vector<int> child;
		for (auto j = 0; j < size; j++) {
			int count = 0;
			if (i > 0) {
				if (j > 0) {
					count += vector[i - 1][j - 1];
				}
				if (j < size) {
					count += vector[i - 1][j + 1];
				}
				count += vector[i - 1][j];
			}
			if (i < size - 1) {
				if (j > 0) {
					count += vector[i + 1][j - 1];
				}
				if (j < size) {
					count += vector[i + 1][j + 1];
				}
				count += vector[i + 1][j];
			}
			if (j > 0) {
				count += vector[i][j - 1];
			}
			if (j < size) {
				count += vector[i][j + 1];
			}
			if ((vector[i][j] == 0 && count == 3) || (vector[i][j] == 1 && (count == 2 || count == 3))) {
				child.push_back(1);
			} else {
				child.push_back(0);
			}
		}
		vec.push_back(child);
	}
	return vec;
}

class game_of_life {
	private:
		std::vector<std::vector<int>> field;
		int size;
		callback func;
	public:
		game_of_life(std::vector<std::vector<int>> arg0, int size, callback arg1);
		void run();
		void console_clear();
		void draw_blocks();
};

game_of_life::game_of_life(std::vector<std::vector<int>> arg0, int size, callback arg1) {
	this->field = arg0; // copy
	this->size = size;
	this->func = arg1;
}

void game_of_life::console_clear() {
	std::cout << "\e[;H\e[2J"; // clear screen
}

void game_of_life::draw_blocks() {
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			std::cout << ((this->field[i][j] == 1) ? "\x1b[41m  " : "\x1b[49m  ");
			//std::cout << ((i + j + n) % 2 == 0 ? "\x1b[49m  " : "\x1b[41m  "); // draw color by background color and print double spaces
		}
		std::cout << "\x1b[49m" << std::endl; // reset background color
	}
}

void game_of_life::run() {
	while (true) {
		this->console_clear();
		this->draw_blocks();
		std::chrono::milliseconds dura( 500 ); // 1000 milli second
		std::this_thread::sleep_for( dura ); // wait
		this->field = this->func(this->field, this->size);
	}
}

std::vector<std::vector<int>> initialField(int size) {
	std::vector<std::vector<int>> vector(0);
	int num = 0;
	for (int i = 0; i < size; i++) {
		std::vector<int> child;
		for (int j = 0; j < size; j++) {
			child.push_back(num);
			num = 1 - num;
		}
		vector.push_back(child);
		//num = 1 - num;
	}
	return vector;
}

int main(int argc, char const* argv[])
{
	callback func;
	int size = 40;
	std::vector<std::vector<int>> vec = initialField(size);
	game_of_life test(vec, size, func);
	test.run();
	return 0;
}
