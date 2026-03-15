#include "arena.h"

namespace fex {

Arena::Arena(size_t chunkSize) : m_size(chunkSize) {
    m_buffer = new std::byte[m_size];
    m_offset = m_buffer;
}

Arena::~Arena() {
    delete[] m_buffer;
}

} // namespace fex
