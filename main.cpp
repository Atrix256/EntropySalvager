#include <stdio.h>
#include <vector>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf(
			"EntropySalvager v1.0\n"
			"Turn otherwise useless text into unbiased uniform random numbers.\n"
			"Usage: EntropySalvager.exe <file>"
		);
		return 1;
	}

	// open the file
	FILE* file = nullptr;
	fopen_s(&file, argv[1], "rb");
	if (!file)
	{
		printf("Could not open file for read: %s\n", argv[1]);
		return 1;
	}

	// read the file in
	std::vector<uint8_t> data;
	fseek(file, 0, SEEK_END);
	data.resize(ftell(file));
	fseek(file, 0, SEEK_SET);
	fread(data.data(), data.size(), 1, file);
	fclose(file);

	// create random bits
	std::vector<char> output;
	size_t zeros = 0;
	size_t ones = 0;
	for (uint8_t c : data)
	{
		for (int i = 0; i < 4; ++i)
		{
			switch ((c>>(i*2)) & 3)
			{
				case 1: output.push_back('0'); zeros++; break;
				case 2: output.push_back('1'); ones++; break;
			}
		}
	}

	// output the results
	fopen_s(&file, "out.txt", "wt");
	if (!file)
	{
		printf("could not open file for write: out.dat\n");
		return 1;
	}
	fwrite(output.data(), output.size(), 1, file);
	fclose(file);

	// report stats
	printf(
		"Entropy Salvager v1.0\n\n"
		"Input       : %zu bytes\n"
		"Output bits : %zu random bits (%zu zeros and %zu ones)\n"
		"Output bytes: %0.2f bytes\n"
		"Efficiency  : %0.2f%%\n",
		data.size(),
		output.size(),
		zeros,
		ones,
		float(output.size()) / 8.0f,
		100.0f * (float(output.size())/8.0f) / float(data.size())
	);

	return 0;
}