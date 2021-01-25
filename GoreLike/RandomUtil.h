#pragma once

#include <random>
#include <memory>

class RandomUtil {
private:

	std::random_device rd{};
	std::mt19937 gen;

	RandomUtil() {
		gen = std::mt19937(rd());
	}

public:
	static std::shared_ptr<RandomUtil> Get() {
		static std::shared_ptr<RandomUtil> _ru(new RandomUtil);
		return _ru;
	}

	int random_int(int l, int h) {
		return std::uniform_int_distribution<>(l, h)(gen);
	}

};
