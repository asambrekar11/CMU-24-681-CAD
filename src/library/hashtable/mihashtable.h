#ifndef MIHASHTABLE_IS_INCLUDED
#define MIHASHTABLE_IS_INCLUDED

#include <vector>

template <class Key, class Value>
class HashTable
{
protected:
	typedef unsigned long long int Code;
	struct Entry
	{
	public:
		Key hashKey;
		Code hashCode;
		Value value;
	};
	enum
	{
		MIN_TABLE_SIZE = 7
	};
	std::vector <std::vector <Entry>> table;
	Code nelem;
	virtual long long int HashCode(const Key &key) const;
	void Cleanup();
public:
	HashTable();
	void Update(const Key &key, Value &value);
	bool IsIncluded(Key key) const;
	void Resize(Code newSize);
	Value *operator[](const Key key);
	Value *operator[](const Key key) const;
};
#endif
