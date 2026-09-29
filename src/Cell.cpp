#include "Cell.h"

Cell::Cell(const int digit) :
	_digit(digit),
	_is_open(true)
{}

int Cell::GetDigit() const
{
	return _digit;
}

bool Cell::IsLocked() const
{
	return not _is_open;
}

void Cell::SetDigit(const int digit)
{
	_digit = digit;
}

void Cell::Lock()
{
	if (_is_open) {
		_is_open = false;
	}
}

void Cell::Open()
{
	if (not _is_open) {
		_is_open = true;
	}
}

void Cell::Reset()
{
	for (int i = 0; i < 9; i += 1) _free_digits[i] = true; // изначально все свободны
	_digit = 0;
	_is_open=true;
}

void Cell::RemoveFD(int digit)
{
	_free_digits[digit - 1] = false;
}

thread_local std::mt19937 gen(std::random_device{}());

bool Cell::GenerateDigit()
{
	std::vector<int> available;
    for (int i = 0; i < 9; ++i) {
        if (_free_digits[i]) available.push_back(i + 1);
    }

    if (available.empty()) return false;

    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);
    _digit = available[dist(gen)];
    _free_digits[_digit - 1] = false;
    return true;
}
