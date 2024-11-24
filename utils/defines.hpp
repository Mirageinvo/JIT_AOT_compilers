#ifndef UTILS_DEFINES_HPP
#define UTILS_DEFINES_HPP

#define NO_COPY_SEMANTICS(name) \
    name(const name&) = delete; \
    name& operator=(const name&) = delete;

#define NO_MOVE_SEMANTICS(name) \
    name(name&&) = delete;      \
    name& operator=(name&&) = delete;

#define DEFAULT_COPY_SEMANTICS(name) \
    name(const name&) = default;     \
    name& operator=(const name&) = default;

#define DEFAULT_MOVE_SEMANTICS(name) \
    name(name&&) = default;          \
    name& operator=(name&&) = default;

#define NO_COPY_AND_MOVE_SEMANTICS(name) \
    NO_COPY_SEMANTICS(name);             \
    NO_MOVE_SEMANTICS(name);

#define DEFAULT_COPY_AND_MOVE_SEMANTICS(name) \
    DEFAULT_COPY_SEMANTICS(name);             \
    DEFAULT_MOVE_SEMANTICS(name);

#endif  // UTILS_DEFINES_HPP
