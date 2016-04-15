#include "mihashtable.h"

template <class Key, class Value>
void HashTable<Key,Value>::Cleanup()
{
	table.Resize(MIN_TABLE_SIZE);
	for (auto &t : table)
	{
		t.clear();
	}
	nelem = 0;
}

template <class Key, class Value>
HashTable<Key,Value>::HashTable()
{
	table.Resize(MIN_TABLE_SIZE);
	nelem = 0;
}

template <class Key, class Value>
void HashTable<Key,Value>::Update(const Key &key, Value &value)
{
	auto hashCode = HashCode(key);
	auto idx = hashCode%table.size();
	for (auto &t : table[idx])
	{
		if (t.hashCode == hashCode && t.hashKey == key)
		{
			t.value = value;
			return;
		}
	}
	Entry entry;
	entry.hashKey=key;
	entry.hashCode=hashCode;
	entry.value=value;
	table[idx].push_back(entry);
	nelem++;
}

template <class Key, class Value>
bool HashTable<Key,Value>::IsIncluded(Key key) const
{
	auto hashCode = HashCode(key);
	auto idx = hashCode%table.size();
	for (auto &t : table[idx])
	{
		if (t.hashCode == hashCode && t.hashKey == key)
		{
			return true;
		}
	}
	return false;
}

template <class Key, class Value>
void HashTable<Key,Value>::Resize(Code newSize)
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
	table.resize(newSize);
	for(auto b : buffer)
	{
		Update(b.hashKey,b.value);
	}
}

template <class Key, class Value>
Value *HashTable<Key,Value>::operator[](const Key key)
{
	auto hashCode = HashCode(key);
	auto idx = hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			return &e.value;
		}
	}
	return nullptr;
}

template <class Key, class Value>
Value *HashTable<Key,Value>::operator[](const Key key) const
{
		auto hashCode = HashCode(key);
	auto idx = hashCode%table.size();
	for(auto &e : table[idx])
	{
		if(e.hashCode==hashCode && e.hashKey==key)
		{
			return &e.value;
		}
	}
	return nullptr;
}
