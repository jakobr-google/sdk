// Copyright (c) 2014, the Dartino project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

#ifndef SRC_VM_THREAD_CMSIS_H_
#define SRC_VM_THREAD_CMSIS_H_

#ifndef SRC_VM_THREAD_H_
#error "Don't include thread_cmsis.h directly, include thread.h."
#endif

#include <cmsis_os.h>
#include <errno.h>

#include "src/shared/assert.h"

namespace dartino {

// A ThreadIdentifier represents a thread identifier for a thread.
// The ThreadIdentifier does not own the underlying OS handle.
// Thread handles can be used for referring to threads and testing equality.
class ThreadIdentifier {
 public:
  ThreadIdentifier() : thread_(osThreadGetId()) {}

  // Test for thread running.
  bool IsSelf() const { return osThreadGetId() == thread_; }

  // Try to join the thread identified by this [ThreadIdentifier].
  //
  // A thread can only be joined once.
  void Join() {
    printf("ThreadIdentifier::Join is not supported on CMSIS.\n");
    fflush(stdout);
  }

 private:
  friend class Thread;

  explicit ThreadIdentifier(osThreadId thread) : thread_(thread) {}

  osThreadId thread_;
};

}  // namespace dartino

#endif  // SRC_VM_THREAD_CMSIS_H_
