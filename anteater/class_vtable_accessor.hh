#pragma once

template<size_t k_class_vtable_size>
class c_class_vtable_accessor
{
public:
	template<class class_t>
	__forceinline c_class_vtable_accessor(
		class_t* class_pointer) :
		m_vtable_data(*reinterpret_cast<void***>(class_pointer))
	{
		VirtualProtect(
			m_vtable_data,
			k_class_vtable_size,
			PAGE_READWRITE,
			&m_old_protection);
	}

	__forceinline ~c_class_vtable_accessor()
	{
		VirtualProtect(
			m_vtable_data,
			k_class_vtable_size,
			m_old_protection,
			&m_old_protection);
	}

	template<typename function_t>
	__forceinline void replace_at_index(
		size_t index,
		function_t** old_pointer,
		function_t* new_pointer)
	{
		*old_pointer = reinterpret_cast<function_t*>(m_vtable_data[index]);
		m_vtable_data[index] = reinterpret_cast<void*>(new_pointer);
	}
private:
	void** m_vtable_data;
	DWORD m_old_protection;
};
