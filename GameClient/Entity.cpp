#include "pch.h"
#include "Entity.h"

UINT Entity::g_nextID = 0;

Entity::Entity()
	: m_instID(++g_nextID), m_name(), m_refCount(0)
{
}

Entity::Entity(const Entity& other)
	: m_instID(++g_nextID), m_name(other.m_name), m_refCount(0)
{

}

Entity::~Entity()
{
}
