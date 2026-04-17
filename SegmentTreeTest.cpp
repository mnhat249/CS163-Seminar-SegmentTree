#include "SegmentTree.h"

#include <iostream>
#include <vector>

bool smallTest() {
	std::vector<long long> values{1, 2, 3, 4, 5};
	SegmentTree tree(values);

	if (tree.query(1, 0, tree.size() - 1, 0, 4) != 15) {
		std::cout << "Test 1 failed\n";
		return false;
	}

	tree.update(1, 0, tree.size() - 1, 2, 10);
	if (tree.query(1, 0, tree.size() - 1, 0, 4) != 22) {
		std::cout << "Test 2 failed\n";
		return false;
	}

	tree.updateRange(1, 0, tree.size() - 1, 1, 3, 1);
	if (tree.query(1, 0, tree.size() - 1, 0, 4) != 25) {
		std::cout << "Test 3 failed\n";
		return false;
	}

	tree.Delete(4);
	if (tree.query(1, 0, tree.size() - 1, 0, 4) != 20) {
		std::cout << "Test 4 failed\n";
		return false;
	}

	std::cout << "All small tests passed\n";
	return true;
}
