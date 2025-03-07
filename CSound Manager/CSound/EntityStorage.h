#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

template <class T>
class EntityStorage
{
	public:
		EntityStorage(const char *prefix = "prefix");
		~EntityStorage() {};

		void Clear();

		T* Get(const char* name) const;
		T* Create(const char* name);
		std::string Create();
		const std::vector<T*>& GetEntries() const;

		void Set(const char* name, T* entity);

		bool Rename(const char* name, const char* newName);
		void Remove(const char* name);

	private:
		void RebuildList();

	protected:
		const std::string prefix;
		std::unordered_map <std::string, T*> storage;
		std::vector <T*> list;
};

template<class T>
inline EntityStorage<T>::EntityStorage(const char * prefix)
	: prefix(prefix)
{
}

template<class T>
inline void EntityStorage<T>::Clear()
{
	storage.clear();
	list.clear();
}

template<class T>
inline void EntityStorage<T>::Set(const char * name, T * entity)
{
	storage[name] = entity;
	list.push_back(entity);
}

template<class T>
inline T * EntityStorage<T>::Get(const char * name) const
{
	if (storage.find(name) != storage.end()) {
		return storage.at(name);
	}
	#ifdef _DEBUG
	std::cout << "Template: [" << name << "] was not found " << std::endl;
	#endif

	return nullptr;
}

template<class T>
inline bool EntityStorage<T>::Rename(const char * name, const char * newName)
{
	auto pit = storage.find(name);
	auto nit = storage.find(newName);
	if (nit != storage.end() || pit == nit)
		return false;

	storage[newName] = storage[name];
	storage.erase(name);

	return true;
}

template<class T>
inline void EntityStorage<T>::Remove(const char * name)
{
	storage.erase(name);
	RebuildList();
}

template<class T>
inline void EntityStorage<T>::RebuildList()
{
	list.clear();
	for (auto item : storage) {
		list.push_back(item.second);
	}
}

template<class T>
inline T * EntityStorage<T>::Create(const char * name)
{
	if (storage.find(name) != storage.end())
		return nullptr;

	T* entity = new T();
	storage[name] = entity;
	list.push_back(entity);
	return entity;
}

template<class T>
inline std::string EntityStorage<T>::Create()
{
	std::string name;
	T* entry = nullptr;
	unsigned int ID = 0;

	while (entry == nullptr)
	{
		name = prefix;
		name.append(std::to_string(ID));
		entry = Create(name.c_str());
		ID++;
	}

	return name;
}

template<class T>
inline const std::vector<T*>& EntityStorage<T>::GetEntries() const
{
	return list;
}
