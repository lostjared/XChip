#include <cstddef>
#include <cstdlib>
#include <iostream>

void* operator new(std::size_t size)
{
	std::size_t* block = (std::size_t*) std::malloc(size + sizeof(std::size_t));
	*block = size;
	return (block + 1);
}

void operator delete(void* block)
{
	free(((std::size_t*)block) - 1);

}

void operator delete[](void* block)
{
	free(((std::size_t*)block) - 1);
}


template<class T>
std::size_t get_arr_size(T* arr)
{
	std::size_t* size = reinterpret_cast<std::size_t*>(arr);
	return *(size - 1) / sizeof(T);
}



int main(void)
{
	while(true) {
		int* arr = new int[100];
		char* arr2 = new char[500];	
		delete[] arr;
		delete[] arr2;
	}
}
























