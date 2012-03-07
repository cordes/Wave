class DataChunk {

  public:

	DataChunk(int num, int samplesPerSecond);

	short *data;
	int length;
	int samplesPerSecond;

	void add(DataChunk *newChunk, int offsetMSeconds);

  private:

};
