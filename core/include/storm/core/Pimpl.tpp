#include "Pimpl.hpp"

namespace storm::core {
	template<typename T, bool initialize>
	Pimpl<T, initialize>::Pimpl() {
		if constexpr (initialize) m_ptr = std::make_unique<T>();
    }

	template<typename T, bool initialize>
    template<typename ...Args>
	Pimpl<T, initialize>::Pimpl(Args&& ...args) {
		if constexpr (initialize) m_ptr = std::make_unique<T>(std::forward<Args>(args)...);
    }

	template<typename T, bool initialize>
	template<typename ...Args>
	inline void Pimpl<T, initialize>::init(Args&& ...args) {
		m_ptr = std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, bool initialize>
	inline T *Pimpl<T, initialize>::operator->() noexcept {
        return m_ptr.get();
    }

	template<typename T, bool initialize>
	inline T const *Pimpl<T, initialize>::operator->() const noexcept{
        return m_ptr.get();
    }

	template<typename T, bool initialize>
	inline T &Pimpl<T, initialize>::operator*() noexcept {
        return *m_ptr;
    }

	template<typename T, bool initialize>
	inline T const &Pimpl<T, initialize>::operator*() const noexcept {
        return std::as_const(*m_ptr);
    }

	template<typename T, bool initialize>
	inline T *Pimpl<T, initialize>::get() noexcept {
		return m_ptr.get();
	}

	template<typename T, bool initialize>
	inline const T * const Pimpl<T, initialize>::get() const noexcept {
		return std::as_const(m_ptr.get());
	}

}
