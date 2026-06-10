/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CircularBuffer.cpp
*/

#include "Server/CircularBuffer.hpp"
#include <algorithm>

CircularBuffer::CircularBuffer(size_t capacity)
    : m_buffer(capacity), m_head(0), m_tail(0), m_full(false), m_capacity(capacity)
{
}

size_t CircularBuffer::size() const
{
    if (m_full)
        return m_capacity;
    if (m_head >= m_tail)
        return m_head - m_tail;
    return m_capacity + m_head - m_tail;
}

size_t CircularBuffer::available_space() const
{
    return m_capacity - size();
}

void CircularBuffer::clear()
{
    m_head = 0;
    m_tail = 0;
    m_full = false;
}

void CircularBuffer::append(const std::string& data)
{
    if (data.empty())
        return;

    // If data is larger than capacity or available space, we clear buffer as per user preference (DoS protection)
    if (data.size() > available_space()) {
        clear();
    }

    // Only copy what fits (if a single packet is larger than capacity, we truncate it)
    size_t to_write = std::min(data.size(), m_capacity);
    size_t data_offset = data.size() - to_write;

    for (size_t i = 0; i < to_write; ++i) {
        m_buffer[m_head] = data[data_offset + i];
        m_head = (m_head + 1) % m_capacity;
    }

    if (size() == m_capacity) {
        m_full = true;
    }
}

bool CircularBuffer::extract_line(std::string& line)
{
    if (is_empty())
        return false;

    size_t current = m_tail;
    size_t length = 0;
    bool found = false;

    while (length < size()) {
        if (m_buffer[current] == '\n') {
            found = true;
            break;
        }
        current = (current + 1) % m_capacity;
        length++;
    }

    if (!found)
        return false;

    line.clear();
    line.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        line.push_back(m_buffer[m_tail]);
        m_tail = (m_tail + 1) % m_capacity;
    }

    // Skip the '\n'
    m_tail = (m_tail + 1) % m_capacity;
    m_full = false;

    // Remove '\r' if present
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    return true;
}

bool CircularBuffer::is_empty() const
{
    return (!m_full && (m_head == m_tail));
}

std::string CircularBuffer::peek_all() const
{
    if (is_empty())
        return "";

    size_t s = size();
    std::string result;
    result.reserve(s);

    size_t tmp_tail = m_tail;
    for (size_t i = 0; i < s; ++i) {
        result.push_back(m_buffer[tmp_tail]);
        tmp_tail = (tmp_tail + 1) % m_capacity;
    }

    return result;
}
