#pragma once
template <typename T>
class Ptr
{
private:
	T* m_ptr;

public:
	T* Get() const { return m_ptr; }
	T** GetAddressOf() const { return &m_ptr; }
	
public:
	Ptr<T>& operator=(T* _ptr)
	{
		if (m_ptr != nullptr)
			m_ptr->Release();

		m_ptr = _ptr;

		if (m_ptr != nullptr)
			m_ptr->AddRef();
		
		return *this;
	}
	
	Ptr<T>& operator=(const Ptr<T>& _ptr)
	{
		if (m_ptr != nullptr)
			m_ptr->Release();

		m_ptr = _ptr.m_ptr;

		if (m_ptr != nullptr)
			m_ptr->AddRef();

		return *this;
	}

	T* operator->() const { return m_ptr; }
	
	bool operator==(T* _ptr) const { return m_ptr == _ptr; }
	bool operator==(const Ptr<T>& _ptr) const { return m_ptr == _ptr.m_ptr; }
	
	bool operator!=(T* _ptr) const { return m_ptr != _ptr; }
	bool operator!=(const Ptr<T>& _ptr) const { return m_ptr != _ptr.m_ptr; }

public:
	Ptr() : m_ptr(nullptr) {}

	Ptr(T* _ptr)
		: m_ptr(_ptr)
	{
		if (m_ptr != nullptr)
			m_ptr->AddRef();
	}

	Ptr(const Ptr<T>& _ptr)
		: m_ptr(_ptr.m_ptr)
	{
		if (m_ptr != nullptr)
			m_ptr->AddRef();
	}

	~Ptr()
	{
		if (m_ptr != nullptr)
			m_ptr->Release();
	}
};

template <typename T>
bool operator==(void* _address, const Ptr<T>& _ptr)
{
	return _address == _ptr.Get();
}

template<typename T>
bool operator!=(void* _address, const Ptr<T>& _ptr)
{
	return _address != _ptr.Get();
}