#ifndef SYSTEM_DYNAMICARRAY_HPP
#define SYSTEM_DYNAMICARRAY_HPP

#include <vector>

namespace System {

template <typename T>
class DynamicArray : public std::vector<T> {

public:

	int get_length() const
	{
		return this->size();
	}

	void set_length(int new_length)
	{
		this->resize(new_length);
	}

	int GetLength() const
	{
		return this->size();
	}

	int SetLength(int NewSize)
	{
		this->resize(NewSize);
	}

};

}

#endif

