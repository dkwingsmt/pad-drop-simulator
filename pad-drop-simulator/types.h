#pragma once

#include "stdafx.h"

typedef int_fast8_t tCell;

const size_t TABLE_WIDTH_MEM_SL = 3;
const size_t TABLE_WIDTH_MEM = 1 << TABLE_WIDTH_MEM_SL;
const size_t TABLE_HEIGHT_MEM = 5;
const size_t TABLE_CELL_NUM_MEM = TABLE_WIDTH_MEM * TABLE_HEIGHT_MEM;

const size_t TABLE_WIDTH = 6;
const size_t TABLE_HEIGHT = 5;

const tCell CELL_MASK_TYPE = 0x0F;
const tCell CELL_MASK_VAR = (tCell)0xF0;

const tCell CELL_MASK_HOLLOW = 0x10;
const tCell CELL_MASK_EMPTY = 0x20;
const tCell CELL_MASK_HOLLOW_TYPE_EMPTY = CELL_MASK_HOLLOW | CELL_MASK_EMPTY | CELL_MASK_TYPE;
const tCell CELL_MASK_HOLLOW_AND_EMPTY = CELL_MASK_HOLLOW | CELL_MASK_EMPTY;

const size_t MIN_STREAK = 3;

const tCell CELL_EMPTY = -1;

template <bool transposed> inline size_t versTablePos(size_t iRow, size_t iCol) {
	if (transposed) {
		return iCol << TABLE_WIDTH_MEM_SL | iRow;
	}
	else {
		return iRow << TABLE_WIDTH_MEM_SL | iCol;
	}
}

template<bool transposed>
constexpr size_t versTableHeightMem() {
	return transposed ? TABLE_WIDTH_MEM : TABLE_HEIGHT_MEM;
}
template<bool transposed>
constexpr size_t versTableWidthMem() {
	return transposed ? TABLE_HEIGHT_MEM : TABLE_WIDTH_MEM;
}
template<bool transposed>
constexpr size_t versTableHeight() {
	return transposed ? TABLE_WIDTH : TABLE_HEIGHT;
}
template<bool transposed>
constexpr size_t versTableWidth() {
	return transposed ? TABLE_HEIGHT : TABLE_WIDTH;
}