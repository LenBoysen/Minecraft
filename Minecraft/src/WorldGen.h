#pragma once
#include "FastNoise/FastNoise.h"


enum class NoiseType
{
	Value,
	Perlin,
	Simplex,
	OpenSimplex2,
	OpenSimplex2S,
	Cellular,
	ValueCubic,
	EnumMax
};

class NoiseGenerator {
public:
	NoiseGenerator();

	bool SetupNoise(FastNoise::SmartNode<>& fastnoise, NoiseType noiseType = NoiseType::Perlin)
	{
		FastSIMD::eLevel elevel = FastSIMD::eLevel::Level_AVX512;

		switch (noiseType)
		{
		case NoiseType::Value: fastnoise = FastNoise::New<FastNoise::Value>(elevel); break;
		case NoiseType::Perlin: fastnoise = FastNoise::New<FastNoise::Perlin>(elevel); break;
		case NoiseType::Simplex: fastnoise = FastNoise::New<FastNoise::Simplex>(elevel); break;
		case NoiseType::OpenSimplex2: fastnoise = FastNoise::New<FastNoise::OpenSimplex2>(elevel); break;
			//case NoiseType::OpenSimplex2S: FastNoise::New<FastNoise::Value>(testSIMD) break;
		case NoiseType::Cellular: fastnoise = FastNoise::New<FastNoise::CellularDistance>(elevel); break;
			//case NoiseType::ValueCubic: FastNoise::New<FastNoise::Value>(testSIMD) break;
		default: return false;
		}
		return true;
	}
	bool IsSupported(NoiseType noiseType, int dimensionCount)
	{
		FastNoise::SmartNode<> fastnoise;
		return SetupNoise(fastnoise, noiseType);
	}
	bool Generate2D(NoiseType noiseType, int dimensionSize, std::vector<float>& noiseOutput)
	{

		FastNoise::SmartNode<> fastnoise;
		if (!SetupNoise(fastnoise, noiseType)) return false;


		int64_t itemIncrement = dimensionSize * dimensionSize;
		int64_t itemCount = 0;
		noiseOutput.reserve(itemIncrement);

		
		fastnoise->GenUniformGrid2D(noiseOutput.data(), 0, 0, dimensionSize, dimensionSize, 0.02f, 1337);

		itemCount += itemIncrement;
	
		

		return true;
	}

private:

private:
	static NoiseGenerator s_NoiseGenerator;
};