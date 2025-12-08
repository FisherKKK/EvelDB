#ifndef STORAGE_EVELDB_PORT_THREAD_ANNOTATIONS_H_
#define STORAGE_EVELDB_PORT_THREAD_ANNOTATIONS_H_

#if defined(__clang__)
#define THREAD_ANNOTATION_ATTRIBUTE__(x) __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)  // no-op
#endif

#ifndef LOCKABLE
#define LOCKABLE THREAD_ANNOTATION_ATTRIBUTE__(lockable)
#endif

#endif