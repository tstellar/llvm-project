//===--------------------------- new.cpp ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stdlib.h>

#include "new"
#include "include/atomic_support.h"

#if defined(_LIBCPP_ABI_MICROSOFT)
#   if !defined(_LIBCPP_ABI_VCRUNTIME)
#       include "support/runtime/new_handler_fallback.ipp"
#   endif
#elif defined(LIBCXX_BUILDING_LIBCXXABI)
#   include <cxxabi.h>
#elif defined(LIBCXXRT)
#   include <cxxabi.h>
#   include "support/runtime/new_handler_fallback.ipp"
#elif defined(__GLIBCXX__)
    // nothing to do
#else
#   include "support/runtime/new_handler_fallback.ipp"
#endif

namespace std
{

#ifndef __GLIBCXX__
const nothrow_t nothrow{};
#endif

#ifndef LIBSTDCXX

void
__throw_bad_alloc()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    throw bad_alloc();
#else
    _VSTD::abort();
#endif
}

#endif // !LIBSTDCXX

}  // std

#if !defined(__GLIBCXX__) &&                                                   \
    !defined(_LIBCPP_ABI_VCRUNTIME) &&      \
    !defined(_LIBCPP_DISABLE_NEW_DELETE_DEFINITIONS)

// Implement all new and delete operators as weak definitions
// in this shared library, so that they can be overridden by programs
// that define non-weak copies of the functions.

_LIBCPP_WEAK
void *
operator new(std::size_t size) _THROW_BAD_ALLOC
{
    if (size == 0)
        size = 1;
    void* p;
    while ((p = ::malloc(size)) == 0)
    {
        // If malloc fails and there is a new_handler,
        // call it to try free up memory.
        std::new_handler nh = std::get_new_handler();
        if (nh)
            nh();
        else
#ifndef _LIBCPP_NO_EXCEPTIONS
            throw std::bad_alloc();
#else
            break;
#endif
    }
    return p;
}

_LIBCPP_WEAK
void*
operator new(size_t size, const std::nothrow_t&) _NOEXCEPT
{
    void* p = 0;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new(size);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void*
operator new[](size_t size) _THROW_BAD_ALLOC
{
    return ::operator new(size);
}

_LIBCPP_WEAK
void*
operator new[](size_t size, const std::nothrow_t&) _NOEXCEPT
{
    void* p = 0;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new[](size);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void
operator delete(void* ptr) _NOEXCEPT
{
<<<<<<< HEAD
    ::free(ptr);
=======
    if (ptr)
        ::free(ptr);
>>>>>>> origin/release/4.x
}

_LIBCPP_WEAK
void
operator delete(void* ptr, const std::nothrow_t&) _NOEXCEPT
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, size_t) _NOEXCEPT
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr) _NOEXCEPT
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, const std::nothrow_t&) _NOEXCEPT
{
    ::operator delete[](ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, size_t) _NOEXCEPT
{
    ::operator delete[](ptr);
}

<<<<<<< HEAD
#if !defined(_LIBCPP_HAS_NO_LIBRARY_ALIGNED_ALLOCATION)
=======
#if !defined(_LIBCPP_HAS_NO_ALIGNED_ALLOCATION)
>>>>>>> origin/release/4.x

_LIBCPP_WEAK
void *
operator new(std::size_t size, std::align_val_t alignment) _THROW_BAD_ALLOC
{
    if (size == 0)
        size = 1;
    if (static_cast<size_t>(alignment) < sizeof(void*))
      alignment = std::align_val_t(sizeof(void*));
    void* p;
<<<<<<< HEAD
#if defined(_LIBCPP_MSVCRT_LIKE)
=======
#if defined(_LIBCPP_MSVCRT)
>>>>>>> origin/release/4.x
    while ((p = _aligned_malloc(size, static_cast<size_t>(alignment))) == nullptr)
#else
    while (::posix_memalign(&p, static_cast<size_t>(alignment), size) != 0)
#endif
    {
        // If posix_memalign fails and there is a new_handler,
        // call it to try free up memory.
        std::new_handler nh = std::get_new_handler();
        if (nh)
            nh();
        else {
#ifndef _LIBCPP_NO_EXCEPTIONS
            throw std::bad_alloc();
#else
            p = nullptr; // posix_memalign doesn't initialize 'p' on failure
            break;
#endif
        }
    }
    return p;
}

_LIBCPP_WEAK
void*
operator new(size_t size, std::align_val_t alignment, const std::nothrow_t&) _NOEXCEPT
{
    void* p = 0;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new(size, alignment);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void*
operator new[](size_t size, std::align_val_t alignment) _THROW_BAD_ALLOC
{
    return ::operator new(size, alignment);
}

_LIBCPP_WEAK
void*
operator new[](size_t size, std::align_val_t alignment, const std::nothrow_t&) _NOEXCEPT
{
    void* p = 0;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif  // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new[](size, alignment);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif  // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void
operator delete(void* ptr, std::align_val_t) _NOEXCEPT
{
#if defined(_LIBCPP_MSVCRT_LIKE)
    ::_aligned_free(ptr);
#else
    ::free(ptr);
#endif
}

_LIBCPP_WEAK
void
operator delete(void* ptr, std::align_val_t alignment, const std::nothrow_t&) _NOEXCEPT
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, size_t, std::align_val_t alignment) _NOEXCEPT
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, std::align_val_t alignment) _NOEXCEPT
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, std::align_val_t alignment, const std::nothrow_t&) _NOEXCEPT
{
    ::operator delete[](ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, size_t, std::align_val_t alignment) _NOEXCEPT
{
    ::operator delete[](ptr, alignment);
}

<<<<<<< HEAD
#endif // !_LIBCPP_HAS_NO_LIBRARY_ALIGNED_ALLOCATION
#endif // !__GLIBCXX__ && !_LIBCPP_ABI_VCRUNTIME && !_LIBCPP_DISABLE_NEW_DELETE_DEFINITIONS
=======
#endif // !defined(_LIBCPP_HAS_NO_ALIGNED_ALLOCATION)

#endif // !__GLIBCXX__

namespace std
{

#ifndef __GLIBCXX__
const nothrow_t nothrow = {};
#endif

#ifndef _LIBCPPABI_VERSION

#ifndef __GLIBCXX__

new_handler
set_new_handler(new_handler handler) _NOEXCEPT
{
    return __sync_lock_test_and_set(&__new_handler, handler);
}

new_handler
get_new_handler() _NOEXCEPT
{
    return __sync_fetch_and_add(&__new_handler, nullptr);
}

#endif // !__GLIBCXX__

#ifndef LIBCXXRT

bad_alloc::bad_alloc() _NOEXCEPT
{
}

#ifndef __GLIBCXX__

bad_alloc::~bad_alloc() _NOEXCEPT
{
}

const char*
bad_alloc::what() const _NOEXCEPT
{
    return "std::bad_alloc";
}

#endif // !__GLIBCXX__

bad_array_new_length::bad_array_new_length() _NOEXCEPT
{
}

#ifndef __GLIBCXX__

bad_array_new_length::~bad_array_new_length() _NOEXCEPT
{
}

const char*
bad_array_new_length::what() const _NOEXCEPT
{
    return "bad_array_new_length";
}

#endif // !__GLIBCXX__

#endif //LIBCXXRT

bad_array_length::bad_array_length() _NOEXCEPT
{
}

#ifndef __GLIBCXX__

bad_array_length::~bad_array_length() _NOEXCEPT
{
}

const char*
bad_array_length::what() const _NOEXCEPT
{
    return "bad_array_length";
}

#endif // !__GLIBCXX__

#endif // _LIBCPPABI_VERSION

#ifndef LIBSTDCXX

void
__throw_bad_alloc()
{
#ifndef _LIBCPP_NO_EXCEPTIONS
    throw bad_alloc();
#else
    _VSTD::abort();
#endif
}

#endif // !LIBSTDCXX

}  // std
>>>>>>> origin/release/4.x
