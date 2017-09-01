// pad-drop-simulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "types.h"
#include "utils.h"
#include "calculate.h"
#include "calculate_test.h"

bool increase(size_t *arr, size_t len, size_t max) {
	for (size_t i = 0; i < len; i++) {
		size_t baseIdx = len - 1 - i;
		if (arr[baseIdx] != max - 1 - i) {
			size_t baseVal = arr[baseIdx] + 1;
			for (size_t j = 0; j <= i; j++) {
				arr[baseIdx + j] = baseVal + j;
			}
			return true;
		}
	}
	return false;
}

void generateTable(const size_t *colorPoss, size_t len, tCell *table) {
	size_t idx = 0;
	size_t pos = 0;
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++, idx++, pos++) {
			if (len >= 0 && *colorPoss == pos) {
				colorPoss++;
				len--;
				table[idx] = 1;
			}
			else {
				table[idx] = 0;
			}
		}
		idx += TABLE_WIDTH_MEM - TABLE_WIDTH;
	}
}

void findBestCombo(int oneColor) {
	size_t colorPoss[TABLE_WIDTH * TABLE_HEIGHT];
	for (size_t i = 0; i < oneColor; i++) {
		colorPoss[i] = i;
	}

	tCell table[TABLE_CELL_NUM_MEM];
	size_t currentMaxCombo = 0;
	size_t currentNum = 0;
	do {
		generateTable(colorPoss, oneColor, table);
		size_t combo = calculateCombo(table);
		if (combo >= currentMaxCombo) {
			if (combo > currentMaxCombo) {
				currentNum = 1;
			}
			else {
				currentNum++;
			}
			currentMaxCombo = combo;
			generateTable(colorPoss, oneColor, table);		// Reset it
			printf("Combo %d #%d:\n", combo, currentNum);
			printTable(table);
		}
	} while (increase(colorPoss, oneColor, TABLE_WIDTH * TABLE_HEIGHT));


	printf("OneColor %d, maxCombo %d with %d solutions\n", oneColor, currentMaxCombo, currentNum);
}

int main()
{
	//runAllTests();
	findBestCombo(15);
    return 0;
}

