#ifndef HASHSET_IS_INCLUDED
#define HASHSET_IS_INCLUDED
/* { */

#include <vector>
#include <stdio.h>



template <class KeyType,class ValueType>
class HashTable
{
public:
	typedef unsigned long long CodeType;

	class EnumHandle
	{
	public:
		long long int hashIdx;
		long long int arrayIdx;
	};

private:
	class Entry
	{
	public:
		KeyType hashKey;
		CodeType hashCode;
		ValueType value;
	};

	enum 
	{
		MINIMUM_TABLE_SIZE=7
	};
	std::vector <std::vector <Entry> > table;
	long long int nElem;

public:
	unsigned long long int HashCode(const KeyType &key) const;

	HashTable();
	~HashTable();
	void CleanUp(void);

	void Update(const KeyType &key,const ValueType &value);
	bool IsIncluded(const KeyType &key) const;
	void Resize(long long int tableSize);
	void Delete(const KeyType &key);

	ValueType *operator[](const KeyType key);
	const ValueType *operator[](const KeyType key) const;

	EnumHandle First(void) const;
	bool IsNotNull(EnumHandle hd) const;
	EnumHandle Next(EnumHandle hd) const;
	ValueType *operator[](EnumHandle hd);
	const ValueType *operator[](EnumHandle hd) const;
	const KeyType &GetKey(EnumHandle hd) const;
};

template <class KeyType,class ValueType>
HashTable<KeyType,ValueType>::HashTable()
{
	table.resize(MINIMUM_TABLE_SIZE);
	nElem=0;
}
template <class KeyType,class ValueType>
HashTable<KeyType,ValueType>::~HashTable()
{
}
template <class KeyType,class ValueType>
void HashTable<KeyType,ValueType>::CleanUp(void)
{
	table.resize(MINIMUM_TABLE_SIZE);
	for(auto &t : table)
	{
		t.clear();
	}
	nElem=0;
}
template <class KeyType,class ValueType>
void HashTable<KeyType,ValueType>::Update(const KeyType &key,const ValueType &value)
{
	auto hashCode=HashCode(key);
	auto idx=hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			e.value=value;
			return;
		}
	}
	Entry entry;
	entry.hashKey=key;
	entry.hashCode=hashCode;
	entry.value=value;
	table[idx].push_back(entry);
	++nElem;
}
template <class KeyType,class ValueType>
bool HashTable<KeyType,ValueType>::IsIncluded(const KeyType &key) const
{
	auto hashCode=HashCode(key);
	auto idx=hashCode%table.size();
	for(auto e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			return true;
		}
	}
	return false;
}
template <class KeyType,class ValueType>
void HashTable<KeyType,ValueType>::Resize(long long int tableSize)
{
	std::vector <Entry> buffer;
	for(auto &t : table)
	{
		for(auto e : t)
		{
			buffer.push_back(e);
		}
		t.clear();
	}
	table.resize(tableSize);
	for(auto b : buffer)
	{
		Update(b.hashKey,b.value);
	}
}
template <class KeyType,class ValueType>
void HashTable<KeyType,ValueType>::Delete(const KeyType &key)
{
	auto hashCode=HashCode(key);
	auto idx=hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			e=table[idx].back();
			table[idx].pop_back();
			break;
		}
	}
}

template <class KeyType,class ValueType>
ValueType *HashTable<KeyType,ValueType>::operator[](const KeyType key)
{
	auto hashCode=HashCode(key);
	auto idx=hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			return &e.value;
		}
	}
	return nullptr;
}

template <class KeyType,class ValueType>
const ValueType *HashTable<KeyType,ValueType>::operator[](const KeyType key) const
{
	auto hashCode=HashCode(key);
	auto idx=hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			return &e.value;
		}
	}
	return nullptr;
}

template <class KeyType,class ValueType>
typename HashTable<KeyType,ValueType>::EnumHandle HashTable<KeyType,ValueType>::First(void) const
{
	EnumHandle hd;
	hd.hashIdx=0;
	hd.arrayIdx=0;
	while(hd.hashIdx<table.size())
	{
		if(0<table[hd.hashIdx].size())
		{
			return hd;
		}
		++hd.hashIdx;
	}
	hd.hashIdx=-1;
	hd.arrayIdx=-1;
	return hd;
}
template <class KeyType,class ValueType>
bool HashTable<KeyType,ValueType>::IsNotNull(EnumHandle hd) const
{
	if(hd.hashIdx<0 || table.size()<=hd.hashIdx)
	{
		return false;
	}
	return true;
}
template <class KeyType,class ValueType>
typename HashTable<KeyType,ValueType>::EnumHandle HashTable<KeyType,ValueType>::Next(EnumHandle hd) const
{
	if(true!=IsNotNull(hd))
	{
		return First();
	}

	++hd.arrayIdx;
	if(hd.arrayIdx<table[hd.hashIdx].size())
	{
		return hd;
	}

	hd.arrayIdx=0;
	++hd.hashIdx;
	while(hd.hashIdx<table.size())
	{
		if(0<table[hd.hashIdx].size())
		{
			return hd;
		}
		++hd.hashIdx;
	}

	hd.hashIdx=-1;
	hd.arrayIdx=-1;
	return hd;
}

template <class KeyType,class ValueType>
ValueType *HashTable<KeyType,ValueType>::operator[](EnumHandle hd)
{
	return &table[hd.hashIdx][hd.arrayIdx].value;
}
template <class KeyType,class ValueType>
const ValueType *HashTable<KeyType,ValueType>::operator[](EnumHandle hd) const
{
	return &table[hd.hashIdx][hd.arrayIdx].value;
}

template <class KeyType,class ValueType>
const KeyType &HashTable<KeyType,ValueType>::GetKey(EnumHandle hd) const
{
	return table[hd.hashIdx][hd.arrayIdx].hashKey;
}

/* } */
#endif
