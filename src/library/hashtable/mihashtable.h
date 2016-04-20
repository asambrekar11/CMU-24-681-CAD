#ifndef MIHASHTABLE_IS_INCLUDED
#define MIHASHTABLE_IS_INCLUDED

#include <vector>

template <class Key>
class HashSet
{
protected:
	typedef unsigned long long int big;
	enum
	{
		MIN_TABLE_SIZE = 7
	};
	std::vector <std::vector <Key>> table;
	big nelem;
public:
	void Cleanup();
	HashSet();
	inline ~HashSet()
	{
		Cleanup();
	}
	void Add(const Key &key);
	bool IsIncluded(Key key) const;
	void Resize(big newSize);
	void Delete(Key key);
};


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
public:
	void Cleanup();
	HashTable();
	inline ~HashTable()
	{
		Cleanup();
	}
	void Update(const Key &key, Value &value);
	bool IsIncluded(Key key) const;
	void Resize(Code newSize);
	Value *operator[](const Key key);
	Value *operator[](const Key key) const;
};
#endif
