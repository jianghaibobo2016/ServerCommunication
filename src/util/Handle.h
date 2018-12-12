/*
 * Handle.h
 *
 *  Created on: 2017年7月22日
 *      Author: zxb
 */

#ifndef UTIL_HANDLE_H_
#define UTIL_HANDLE_H_
#include "SystemException.h"
template<typename T>
class Handle {
public:

	Handle(T* p = 0) {
		this->_ptr = p;

		if (this->_ptr) {
			this->_ptr->__incRef();
		}
	}

	template<typename Y>
	Handle(const Handle<Y>& r) {
		this->_ptr = r._ptr;

		if (this->_ptr) {
			this->_ptr->__incRef();
		}
	}

	Handle(const Handle& r) {
		this->_ptr = r._ptr;

		if (this->_ptr) {
			this->_ptr->__incRef();
		}
	}

	~Handle() {
		if (this->_ptr) {
			this->_ptr->__decRef();
		}
	}

	Handle& operator=(T* p) {
		if (this->_ptr != p) {
			if (p) {
				p->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = p;

			if (ptr) {
				ptr->__decRef();
			}
		}
		return *this;
	}

	template<typename Y>
	Handle& operator=(const Handle<Y>& r) {
		if (this->_ptr != r._ptr) {
			if (r._ptr) {
				r._ptr->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = r._ptr;

			if (ptr) {
				ptr->__decRef();
			}
		}
		return *this;
	}

	Handle& operator=(const Handle& r) {
		if (this->_ptr != r._ptr) {
			if (r._ptr) {
				r._ptr->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = r._ptr;

			if (ptr) {
				ptr->__decRef();
			}
		}
		return *this;
	}
	T* get() const {
		return _ptr;
	}

	T* operator->() const {
		if (!_ptr) {
//			throw SystemException(std::string("Handle -> nullptr."));
		}

		return _ptr;
	}

	T& operator*() const {
		if (!_ptr) {
//			throw SystemException(std::string("Handle operator*nullptr."));
		}
		return *_ptr;
	}

	operator bool() const {
		return _ptr ? true : false;
	}
	/* template<typename T, typename U>
	 inline bool operator==(const Handle<T>& lhs, const Handle<U>& rhs)
	 {
	 T* l = lhs.get();
	 U* r = rhs.get();
	 if(l && r)
	 {
	 return *l == *r;
	 }

	 // Note: don't use if { } else { }. This causes lots warnings when
	 // compiling with GCC and optimization enabled. See bug 2330.
	 return !l && !r;
	 }

	 template<typename T, typename U>
	 inline bool operator!=(const Handle<T>& lhs, const Handle<U>& rhs)
	 {
	 return !operator==(lhs, rhs);
	 }*/
	template<class Y>
	static Handle dynamicCast(Y* p) {
		return Handle(dynamic_cast<T*>(p));
	}
private:
	T* _ptr;
};

#endif /* UTIL_HANDLE_H_ */
