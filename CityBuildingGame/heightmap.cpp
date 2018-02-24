#pragma once
#include <vector>
#include <time.h>
#include "heightmap.h"

using namespace std;

Heightmap::Heightmap() {};

void Heightmap::GeneratePerlinNoise(vector<vector<float>> &pHeightmap, int pWidth, int pHeight,
		int maximumHeight, int octaveCount)
	{
		width = pWidth;
		height = pHeight;

		GenerateWhiteNoise(pHeightmap);

		//generate smooth noise
		for (int i = 0; i < octaveCount; i++)
		{
			vector<vector<float>> smoothNoise = vector<vector<float>>(pHeight, vector<float>(pWidth, 0));
			for (auto &j : smoothNoise)
				std::fill(j.begin(), j.end(), 0);
			GenerateSmoothNoise(pHeightmap, smoothNoise, i);
			smoothNoiseList.push_back(smoothNoise);
		}

		CombineNoiseMaps(pHeightmap, octaveCount);

		Rescale(pHeightmap, maximumHeight);
	}
void Heightmap::CombineNoiseMaps(vector<vector<float>> &pHeightmap, int octaveCount)
{
	float persistence = 0.5f;
	float amplitude = 1.0f;

	//blend noise together
	for (auto &i : pHeightmap)
		std::fill(i.begin(), i.end(), 0);

	Heightmap::totalAmplitude = 0.0f;

	float maxValue = 0;
	for (int octave = octaveCount - 1; octave >= 0; octave--)
	{
		amplitude *= persistence;
		Heightmap::totalAmplitude += amplitude;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				pHeightmap[i][j] += smoothNoiseList[octave][i][j] * amplitude;
			}
		}
	}
}

void Heightmap::GenerateWhiteNoise(vector<vector<float>> &pHeightmap)
{
	srand((unsigned)time(0)); // seed random numbers

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pHeightmap[i][j] = (float)rand() / (float)RAND_MAX; // generate random numbers between 0 and 1
		}
	}
}
void Heightmap::GenerateSmoothNoise(vector<vector<float>> &baseNoise,
	vector<vector<float>> &smoothNoise, int octave)
{
	int samplePeriod = 1 << octave; // calculates 2 ^ k
	float sampleFrequency = 1.0f / samplePeriod;

	for (int i = 0; i < height; i++)
	{
		//calculate the horizontal sampling indices
		int sample_i0 = (i / samplePeriod) * samplePeriod;
		int sample_i1 = (sample_i0 + samplePeriod) % height; //wrap around
		float vertical_blend = (i - sample_i0) * sampleFrequency;

		for (int j = 0; j < width; j++)
		{
			//calculate the vertical sampling indices
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % width; //wrap around
			float horizontal_blend = (j - sample_j0) * sampleFrequency;

			//blend the top two corners
			float top = Interpolate(baseNoise[sample_i0][sample_j0],
				baseNoise[sample_i1][sample_j0], vertical_blend);

			//blend the bottom two corners
			float bottom = Interpolate(baseNoise[sample_i0][sample_j1],
				baseNoise[sample_i1][sample_j1], vertical_blend);

			//final blend
			smoothNoise[i][j] = Interpolate(top, bottom, horizontal_blend);
		}
	}
}

float Heightmap::Interpolate(float x0, float x1, float alpha)
{
	return x0 * (1 - alpha) + alpha * x1;
}
void Heightmap::Rescale(vector<vector<float>> &pHeightmap, float maxHeight)
{
	//normalisation
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pHeightmap[i][j] = pHeightmap[i][j] * (maxHeight / totalAmplitude);
		}
	}
}
Heightmap::~Heightmap() {};