#pragma once

#include <cstdlib>
#include <random>
#include <array>

// Данный класс нужен только для того, чтобы создавать и решать судоку
class Cell
{
public:
	Cell(const int digit = 0);

	int GetDigit() const;

	bool IsLocked() const;

	void SetDigit(const int digit);

	void Lock();

	void Open();

	void Reset();

	void RemoveFD(int digit);

	bool GenerateDigit();

private:
	std::array<bool, 9> _free_digits = {true, true, true, true, true, true, true, true, true};
	int _digit;
	bool _is_open;
};
