#include "stdafx.h"
#include "types.h"
#include "utils.h"

#include "calculate.h"

template <bool transposed> void versFlagHollows(tCell *table) {
	auto tablePos = versTablePos<transposed>;
	size_t tableHeight = versTableHeight<transposed>();
	size_t tableWidth = versTableWidth<transposed>();

	size_t streak;
	for (size_t iRow = 0; iRow < tableHeight; iRow++) {
		streak = 0;
		for (size_t iCol = 1; iCol < tableWidth; iCol++) {
			tCell *thisPos = table + tablePos(iRow, iCol);
			tCell *lastPos = table + tablePos(iRow, iCol - 1);
			tCell colJump = thisPos - lastPos;
			if (*thisPos & CELL_MASK_EMPTY) {
				streak = 0;
				continue;
			}

			if ((*thisPos & CELL_MASK_TYPE) == (*lastPos & CELL_MASK_TYPE) && !(*lastPos & CELL_MASK_EMPTY)) {
				streak++;
			}
			else {
				streak = 0;
			}
			if (streak == MIN_STREAK - 1) {
				for (size_t i = 0; i < MIN_STREAK; i++) {
					*thisPos |= CELL_MASK_HOLLOW;
					thisPos -= colJump;
				}
			}
			else if (streak >= MIN_STREAK) {
				*thisPos |= CELL_MASK_HOLLOW;
			}
		}
	}
}

void flagHollows(tCell *table) {
	versFlagHollows<false>(table);
	versFlagHollows<true>(table);
}

size_t findAncester(tCell *library, size_t idx) {
	size_t parent = library[idx];
	if (parent == idx || parent == CELL_EMPTY) {
		return parent;
	}
	parent = findAncester(library, parent);
	library[idx] = (tCell)parent;
	return parent;
}

size_t countHollowGroups(const tCell *table, tCell *library) {
	size_t idx = 0;
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++, idx++) {
			size_t thisVal = table[idx] & CELL_MASK_HOLLOW_TYPE_EMPTY;
			size_t leftIdx = idx - 1;
			size_t topIdx = idx - TABLE_WIDTH_MEM;

			if (!(thisVal & CELL_MASK_HOLLOW) || (thisVal & CELL_MASK_EMPTY)) {
				library[idx] = CELL_EMPTY;
				continue;
			}

			size_t sameAsLeft = iCol > 0 && (thisVal == (table[leftIdx] & CELL_MASK_HOLLOW_TYPE_EMPTY));
			size_t sameAsTop = iRow > 0 && (thisVal == (table[topIdx] & CELL_MASK_HOLLOW_TYPE_EMPTY));
			
			library[idx] = (tCell)idx;
			if (sameAsLeft) {
				size_t ancester = findAncester(library, leftIdx);
				library[idx] = (tCell)ancester;
			}
			if (sameAsTop) {
				size_t ancester = findAncester(library, topIdx);
				if (sameAsLeft) {
					library[ancester] = library[idx];
				}
				else {
					library[idx] = (tCell)ancester;
				}
			}

		}
		idx += TABLE_WIDTH_MEM - TABLE_WIDTH;
	}
	idx = 0;
	size_t count = 0;

	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
			size_t ancester = findAncester(library, idx);
			if (ancester == idx) {
				count++;
			}
			library[idx] = ancester;

			idx++;
		}
		idx += TABLE_WIDTH_MEM - TABLE_WIDTH;
	}

	return count;
}

void simulateDrop(tCell *table) {
	tCell *src = table + TABLE_CELL_NUM_MEM - TABLE_WIDTH_MEM;
	tCell *tgt = src;
	for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
		for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
			while (src >= table && *src & CELL_MASK_HOLLOW_AND_EMPTY) {
				src -= TABLE_WIDTH_MEM;
			}
			if (src < table) {
				*tgt = CELL_MASK_EMPTY;
			}
			else {
				*tgt = *src;
				src -= TABLE_WIDTH_MEM;
			}
			tgt -= TABLE_WIDTH_MEM;
		}

		tgt += TABLE_CELL_NUM_MEM + 1;
		src = tgt;
	}
}

bool libraryHas8(const tCell *library) {
	size_t colorPoss[TABLE_CELL_NUM_MEM];

	size_t idx = 0;
	for (size_t i = 0; i < TABLE_CELL_NUM_MEM; i++) {
		colorPoss[i] = 0;
	}
	for (size_t iRow = 0; iRow < TABLE_HEIGHT; iRow++) {
		for (size_t iCol = 0; iCol < TABLE_WIDTH; iCol++) {
			if (library[idx] != CELL_EMPTY) {
				colorPoss[library[idx]]++;
			}

			idx++;
		}
		idx += TABLE_WIDTH_MEM - TABLE_WIDTH;
	}

	for (size_t i = 0; i < TABLE_CELL_NUM_MEM; i++) {
		if (colorPoss[i] >= 8) {
			return true;
		}
	}
	return false;
}

size_t calculateCombo(tCell *table, const tFilterFunc *filter) {
	size_t thisCount;
	size_t count = 0;

	bool has8 = false;

	while(1) {
		flagHollows(table);

		tCell library[TABLE_CELL_NUM_MEM];
		thisCount = countHollowGroups(table, library);
		if (!thisCount) {
			break;
		}
		has8 |= libraryHas8(library);
		count += thisCount;
		simulateDrop(table);
	}

	if (!has8) {
		return 0;
	}
	return count;
}