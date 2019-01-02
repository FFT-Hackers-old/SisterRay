#include "utils.h"

u16 getRandomBit(u16 bit_vector) {
	while (1) {
		u8 rand_bit = (rand() % 0xB);
		int bit_set = isBitSet(bit_vector, rand_bit);
		if (bit_set) {
			return (u16)(1 << rand_bit);
		}
	}
}

int isBitSet(u16 vector, int bit) {
	return 1 == ((vector >> bit) & 1);
}