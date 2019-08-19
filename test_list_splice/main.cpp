#include <iostream>
#include <list>

std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list)
{
	for (auto& i : list) {
		ostr << " " << i;
	}
	return ostr;
}

int main()
{
	std::list<int> list1 = { 1, 2, 3, 4, 5 };
	std::list<int> list2 = { 10, 20, 30, 40, 50 };

	auto it = list1.begin();
	std::cout << "it1: " << *it << std::endl;
	std::advance(it, 2);
	std::cout << "it1: " << *it << std::endl;

	//list1.splice(it, list2);

	//list1.splice(list1.end(), list1, it);

	std::cout << "list1: " << list1 << "\n";

	//return 0;
	std::cout << "list2: " << list2 << "\n";

	
	list2.splice(list2.begin(), list1, it/*, list1.end()*/);

	std::cout << "list1: " << list1 << "\n";
	std::cout << "list2: " << list2 << "\n";


}