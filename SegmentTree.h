#pragma once

#include <vector>

bool smallTest();

class SegmentTree {
public:
	SegmentTree();
	explicit SegmentTree(const std::vector<long long>& values);

	void setArray(const std::vector<long long>& values);
	void build(int node, int left, int right);
	void update(int node, int left, int right, int pos, long long new_value);
	long long query(int node, int left, int right, int query_L, int query_R);
	void Delete(int pos);
	void updateRange(int node, int left, int right, int update_L, int update_R, long long value);

	int size() const;
	const std::vector<long long>& data() const;

private:
	int n;
	std::vector<long long> array;
	std::vector<long long> tree;
	std::vector<long long> lazy;

	void push(int node, int left, int right);
};
