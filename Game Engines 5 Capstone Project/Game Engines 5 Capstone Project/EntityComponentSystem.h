#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID GetComponentID()
{
	static ComponentID id = 0;
	return id++;
}

template <typename T> inline ComponentID GetComponentID() noexcept
{
	static ComponentID typeID = GetComponentID();
	return typeID;
}

constexpr std::size_t MaxComponents = 32;

using ComponentBitset = std::bitset<MaxComponents>;
using ComponentArray = std::array<Component*, MaxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void PlayerInit() {}
	virtual void UpdatePlayer() {}
	virtual void Draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	bool Active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray comArray;
	ComponentBitset comBitset;

public:
	void Update()
	{
		for (auto& c : components)
		{
			c->UpdatePlayer();
		}
	}

	void Draw() 
	{
		for (auto& c : components)
		{
			c->Draw();
		}
	}

	bool isActive() const { return Active; }
	void DestroyEntity() { Active = false; }

	template <typename T> bool HasComponent() const
	{
		return comBitset[GetComponentID<T>()];
	}

	template <typename T, typename... TArguments>

	T& AddComponent(TArguments&&... args)
	{
		T* c(new T(std::forward<TArguments>(args)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		comArray[GetComponentID<T>()] = c;
		comBitset[GetComponentID<T>()] = true;

		c->PlayerInit();
		return *c;
	}

	template <typename T> T& GetComponent() const
	{
		auto ptr(comArray[GetComponentID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void Update()
	{
		for (auto& e : entities)
		{
			e->Update();
		}
	}

	void Draw()
	{
		for (auto& e : entities)
		{
			e->Draw();
		}
	}

	void Refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& entityManager)
			{
				return !entityManager->isActive();
			}), std::end(entities));
	}

	Entity& AddEntity()
	{
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};