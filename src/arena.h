#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <new>

namespace fex {

class Arena {
  public:
    explicit Arena(size_t chunkSize);
    ~Arena();

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    template <typename T, typename... Args>
    T* Alloc(Args&&... args) {
        size_t space = (m_buffer + m_size) - m_offset;
        void* ptr = static_cast<void*>(m_offset);

        // This adjusts 'ptr' to the next valid alignment for T
        // and decreases 'space' by the amount of padding added.
        if (!std::align(alignof(T), sizeof(T), ptr, space)) {
            throw std::bad_alloc();
        }

        m_offset = static_cast<std::byte*>(ptr) + sizeof(T);
        return new (ptr) T(std::forward<Args>(args)...);
    }

  private:
    const size_t m_size;

    std::byte* m_buffer;
    std::byte* m_offset;
};

} // namespace fex
