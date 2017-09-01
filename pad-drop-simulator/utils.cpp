#include "utils.h"

void printTable(const tCell *table) {
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		std::cout << "| ";
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
			std::cout.width(2);
			std::cout << std::hex << (int)table[versTablePos<false>(iRow, iCol)] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "==========" << std::endl;
}