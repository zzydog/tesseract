/**********************************************************************
 * File:        hashfn.h  (Formerly hash.h)
 * Description: Portability hacks for hash_map, hash_set and unique_ptr.
 * Author:      Ray Smith
 * Created:     Wed Jan 08 14:08:25 PST 2014
 *
 * (C) Copyright 2014, Google Inc.
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 ** http://www.apache.org/licenses/LICENSE-2.0
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 *
 **********************************************************************/

#ifndef           HASHFN_H
#define           HASHFN_H

#if	defined(__cplusplus) && defined(_MSC_VER)	// Visual Studio
	#if (_MSC_VER >= 1700)						// >= Visual c++ 2012
		#include <unordered_map>
		#include <unordered_set>
		using namespace std;
	#elif (_MSC_VER >= 1500 && _MSC_VER < 1700)	// <= Visual c++ 2010
		#include <tr1/unordered_map>
		#include <tr1/unordered_set>
		#include <memory>
		#define HAVE_UNIQUE_PTR
		#define SmartPtr std::unique_ptr
		using namespace std::tr1;
	#else										// <  Visual c++ 2008
		error "The compiler you use is obsolete!"
	#endif										// is Visual Studio
#elif defined(__cplusplus) && defined(__GNUC__)	// is GCC Compiler
	#if (__GNUC__ >= 5)
		#include <unordered_map>
		#include <unordered_set>
		using namespace std;
	#elif ((__GNUC__ >= 4) && (__GNUC_MINOR__ >= 0))
		#include <tr1/unordered_map>
		#include <tr1/unordered_set>
		using namespace std::tr1;
	#else
		error "The compiler you use is obsolete!"
	#endif
#elif defined(__cplusplus) && defined(__clang__)// is Clang Compiler
    #include <unordered_map>
    #include <unordered_set>
    using namespace std;
#else                                           // Unknow C++ Compiler
	#include <hash_map>
	#include <hash_set>
	#define unordered_map hash_map
	#define unordered_set hash_set
#endif  // gcc
    
    
#ifndef HAVE_UNIQUE_PTR
// Trivial smart ptr. Expand to add features of std::unique_ptr as required.
template<class T> class SmartPtr {
public:
	SmartPtr() : ptr_(NULL) {}
	explicit SmartPtr(T* ptr) : ptr_(ptr) {}
	~SmartPtr() { delete ptr_; }

	T* get() const { return ptr_; }
	void reset(T* ptr) {
		if (ptr_ != NULL) delete ptr_;
		ptr_ = ptr;
	}
	bool operator==(const T* ptr) const { return ptr_ == ptr; }
	T* operator->() const { return ptr_; }
private:
	  T* ptr_;
};
#endif  // HAVE_UNIQUE_PTR

#endif  // HASHFN_H
