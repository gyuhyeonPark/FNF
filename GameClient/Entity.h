#pragma once
/// <summary>
/// 모든 클래스들의 가장 윗쪽 부모.
/// 추상 클래스.
/// </summary>
class Entity
{
private:
	static UINT g_nextID;

private:
	const UINT	m_instID;		// 객체마다 가지는 고유 ID 값.
	wstring		m_name;			// 객체마다 이름을 지정할 수 있다. 같을 수 있음
	int m_refCount;				// 참조 카운팅

private:
	void AddRef();
	void Release()	{ if (--m_refCount <= 0) delete this; }

public:
	UINT GetID() const { return m_instID; }
	wstring GetName() const { return m_name; }
	void SetName(const wstring& name) { m_name = name; }
	
public:
	Entity();
	Entity(const Entity& other);
	virtual ~Entity();

	template<typename T>
	friend class Ptr;
};

