#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataChunk.hpp"

DataChunk::DataChunk(int num, int samplesPerSecond) {
	this->length = num * sizeof(short);
	this->samplesPerSecond = samplesPerSecond;
	this->data = (short *) malloc(this->length);
	for (int i = 0; i < num; i++)
		this->data[i] = 0;
}

int max(int a, int b) {
	if (a >= b)
		return a;
	else
		return b;
}

void DataChunk::add(DataChunk *newChunk, int offsetMSeconds) {
	int offsetSamples, offsetBytes, newNumBytes;
	short *old_data;

	offsetSamples = (offsetMSeconds / 1000.0) * this->samplesPerSecond;
	offsetBytes = offsetSamples * sizeof(short);

	newNumBytes = max(this->length, offsetBytes + newChunk->length);

	if (newNumBytes > this->length) {
		old_data = (short *) malloc(newNumBytes);
		memcpy(old_data, this->data, this->length);
		free(this->data);
	}
	else {
		old_data = this->data;
	}

    int numNewChunkSamples = newChunk->length / sizeof(short);
	for (int i = 0; i < numNewChunkSamples; i++) {
		if (i + offsetSamples < this->length)
			old_data[i + offsetSamples] = old_data[i + offsetSamples] * 0.5 + newChunk->data[i] * 0.5;
        else // memcpy?!?!?!?!
			old_data[i + offsetSamples] = newChunk->data[i];
	}

	this->length = newNumBytes;
	this->data = old_data;
}
