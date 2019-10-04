#ifndef TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H
#define TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H

#include "test_macros.h"

#if TEST_STD_VER >= 11
template <class T>
struct EmplaceConstructible {
  T value;
<<<<<<< HEAD
  explicit EmplaceConstructible(T xvalue) : value(xvalue) {}
=======
  explicit EmplaceConstructible(T value) : value(value) {}
>>>>>>> origin/release/5.x
  EmplaceConstructible(EmplaceConstructible const&) = delete;
};

template <class T>
struct EmplaceConstructibleAndMoveInsertable {
  int copied = 0;
  T value;
<<<<<<< HEAD
  explicit EmplaceConstructibleAndMoveInsertable(T xvalue) : value(xvalue) {}
=======
  explicit EmplaceConstructibleAndMoveInsertable(T value) : value(value) {}
>>>>>>> origin/release/5.x

  EmplaceConstructibleAndMoveInsertable(
      EmplaceConstructibleAndMoveInsertable&& Other)
      : copied(Other.copied + 1), value(std::move(Other.value)) {}
};

template <class T>
struct EmplaceConstructibleAndMoveable {
  int copied = 0;
  int assigned = 0;
  T value;
<<<<<<< HEAD
  explicit EmplaceConstructibleAndMoveable(T xvalue) noexcept : value(xvalue) {}
=======
  explicit EmplaceConstructibleAndMoveable(T value) noexcept : value(value) {}
>>>>>>> origin/release/5.x

  EmplaceConstructibleAndMoveable(EmplaceConstructibleAndMoveable&& Other)
      noexcept : copied(Other.copied + 1),
                 value(std::move(Other.value)) {}

  EmplaceConstructibleAndMoveable&
  operator=(EmplaceConstructibleAndMoveable&& Other) noexcept {
    copied = Other.copied;
    assigned = Other.assigned + 1;
    value = std::move(Other.value);
    return *this;
  }
};

template <class T>
struct EmplaceConstructibleMoveableAndAssignable {
  int copied = 0;
  int assigned = 0;
  T value;
<<<<<<< HEAD
  explicit EmplaceConstructibleMoveableAndAssignable(T xvalue) noexcept
      : value(xvalue) {}
=======
  explicit EmplaceConstructibleMoveableAndAssignable(T value) noexcept
      : value(value) {}
>>>>>>> origin/release/5.x

  EmplaceConstructibleMoveableAndAssignable(
      EmplaceConstructibleMoveableAndAssignable&& Other) noexcept
      : copied(Other.copied + 1),
        value(std::move(Other.value)) {}

  EmplaceConstructibleMoveableAndAssignable&
  operator=(EmplaceConstructibleMoveableAndAssignable&& Other) noexcept {
    copied = Other.copied;
    assigned = Other.assigned + 1;
    value = std::move(Other.value);
    return *this;
  }

  EmplaceConstructibleMoveableAndAssignable& operator=(T xvalue) {
    value = std::move(xvalue);
    ++assigned;
    return *this;
  }
};
#endif

#endif // TEST_SUPPORT_EMPLACE_CONSTRUCTIBLE_H
