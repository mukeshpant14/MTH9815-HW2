#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>

using namespace boost::interprocess;

void program1()
{
	managed_shared_memory managed_shm(open_only, "MySharedMemory");
	std::pair<int*, std::size_t> p = managed_shm.find<int>("MyObject");
	if (p.first)
		*p.first = 1000;
}

void program2()
{
	managed_shared_memory managed_shm(open_only, "MySharedMemory");
	std::pair<int*, std::size_t> p = managed_shm.find<int>("MyObject");
	if (p.first)
		*p.first = 5000;
}

int main()
{
	// remove if there is any shared memory with name "MySharedMemory"
	shared_memory_object::remove("MySharedMemory");
	// Create a new shared memory with name "MySharedMemory"
	managed_shared_memory managed_shm( open_or_create, "MySharedMemory", 1024 );

	// create an integer type object 
	int *i = managed_shm.construct<int>("MyObject")(99);

	// program 1 will access it and change the value
	program1();

	std::cout << "After program 1" << std::endl;
	std::pair<int*, std::size_t> p1 = managed_shm.find<int>("MyObject");
	if (p1.first)
		std::cout << *p1.first << '\n';

	// program 2 will access it and change the value
	program2();

	// print the value in shared memory
	std::cout << "After program 1" << std::endl;
	std::pair<int*, std::size_t> p2 = managed_shm.find<int>("MyObject");
	if (p2.first)
		std::cout << *p2.first << '\n';

	std::cout << "\n----------- Press Any key to quit! -------------\n" << std::endl;
	std::cin.get();
}