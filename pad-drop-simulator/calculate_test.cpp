#include "stdafx.h"
#include "types.h"
#include "calculate.h"
#include "utils.h"

#include "calculate_test.h"

void flagHollows(tCell *table);
size_t countHollowGroups(const tCell *table, tCell *filter);
void simulateDrop(tCell *table);

void translateTable(const char *src, tCell *dst) {
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
			dst[versTablePos<false>(iRow, iCol)] = (tCell)*src - (tCell)'0';
			src++;
		}
	}
}

struct ValuePosList
{
	tCell value;
	std::vector<size_t> pos;
};

void tableShouldBe(const tCell *table, tCell defaultVal, const std::vector<ValuePosList> exceptions) {
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
			size_t pos = versTablePos<false>(iRow, iCol);
			bool isException = false;
			for (auto exc : exceptions) {
				for (auto excPos : exc.pos) {
					if (TABLE_WIDTH * iRow + iCol == excPos) {
						assert(table[pos] == exc.value);
						isException = true;
					}
				}
			}
			if (!isException) {
				assert(table[pos] == defaultVal);
			}
		}
	}
}

void test_flagHollows() {
	tCell table[TABLE_CELL_NUM_MEM];

	// All 0
	translateTable(
		"000000"
		"000000"
		"000000"
		"000000"
		"000000", table
	);
	flagHollows(table);
	tableShouldBe(table, 0x10, {});

	// Isolated 
	translateTable(
		"110011"
		"100001"
		"000000"
		"100001"
		"110011", table
	);
	flagHollows(table);
	tableShouldBe(table, 0x10, {
		{0x01, {0, 1, 4, 5, 6, 11, 18, 23, 24, 25, 28, 29}}
	});

	// Isolated 
	translateTable(
		"100001"
		"000010"
		"110010"
		"000000"
		"100001", table
	);
	flagHollows(table);
	tableShouldBe(table, 0x10, {
		{ 0x01, { 0, 5, 10, 12, 13, 16, 24, 29 } }
	});



}

void test_countHollowGroups() {
	tCell table[TABLE_CELL_NUM_MEM];
	tCell library[TABLE_CELL_NUM_MEM];

	// All 0
	translateTable(
		"000000"
		"000000"
		"000000"
		"000000"
		"000000", table
	);
	flagHollows(table);
	assert(countHollowGroups(table, library) == 1);

	// U shape and O shape
	translateTable(
		"000010"
		"000000"
		"000000"
		"100110"
		"100000", table
	);
	flagHollows(table);
	assert(countHollowGroups(table, library) == 1);

	// Snake (late connection)
	translateTable(
		"011111"
		"010001"
		"011101"
		"000001"
		"111111", table
	);
	flagHollows(table);
	assert(countHollowGroups(table, library) == 2);

	// Special (non-hollow connects separate hollows)
	translateTable(
		"011100"
		"110000"
		"100000"
		"100000"
		"000000", table
	);
	flagHollows(table);
	assert(countHollowGroups(table, library) == 3);
}

void test_simulateDrop() {
	tCell table[TABLE_CELL_NUM_MEM];

	// All 0
	translateTable(
		"000000"
		"000000"
		"000000"
		"000000"
		"000000", table
	);
	flagHollows(table);
	simulateDrop(table);
	tableShouldBe(table, 0x20, {});
	
	// Stairs
	translateTable(
		"110022"
		"111002"
		"111100"
		"111112"
		"000000", table
	);
	flagHollows(table);
	simulateDrop(table);
	tableShouldBe(table, 0x20, {
		{ 0x00, { 26, 27, 28,  21, 22, 23 } },
		{ 0x02, { 11, 16, 17, 29 } }
	});

}

void test_calculateCombo() {
	tCell table[TABLE_CELL_NUM_MEM];

	// All 0
	translateTable(
		"000000"
		"000000"
		"000000"
		"000000"
		"000000", table
	);
	assert(calculateCombo(table, nullptr) == 1);

	// 0 combo
	translateTable(
		"010101"
		"101010"
		"010101"
		"101010"
		"010101", table
	);
	assert(calculateCombo(table, nullptr) == 0);

	translateTable(
		"010100"
		"000100"
		"101010"
		"110001"
		"100100", table
	);
	assert(calculateCombo(table, nullptr) == 9);
}

void runAllTests() {
	test_flagHollows();
	test_countHollowGroups();
	test_simulateDrop();
	test_calculateCombo();
}