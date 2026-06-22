/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CircularBuffer.cpp
*/

#include "shared/CircularBuffer.hpp"
#include <algorithm>
#include <stdexcept>

CircularBuffer::CircularBuffer(size_t capacity)
    : _buffer(capacity), _head(0), _tail(0), _full(false), _capacity(capacity)
{
}

size_t CircularBuffer::size() const
{
    if (_full)
        return _capacity;
    if (_head >= _tail)
        return _head - _tail;
    return _capacity + _head - _tail;
}

size_t CircularBuffer::available_space() const
{
    return _capacity - size();
}


void CircularBuffer::resize(size_t new_capacity)
{
    if (new_capacity <= _capacity) return;
    
    std::vector<char> new_buffer(new_capacity);
    size_t current_size = size();
    
    if (current_size > 0) {
        if (_head > _tail || (_head == _tail && !_full)) {
            std::copy(_buffer.begin() + _tail, _buffer.begin() + _tail + current_size, new_buffer.begin());
        } else {
            size_t first_part = _capacity - _tail;
            std::copy(_buffer.begin() + _tail, _buffer.end(), new_buffer.begin());
            std::copy(_buffer.begin(), _buffer.begin() + _head, new_buffer.begin() + first_part);
        }
    }
    
    _buffer = std::move(new_buffer);
    _capacity = new_capacity;
    _tail = 0;
    _head = current_size % _capacity;
    _full = false;
}

void CircularBuffer::clear()
{
    _head = 0;
    _tail = 0;
    _full = false;
}

void CircularBuffer::pop(size_t len)
{
    size_t s = size();
    size_t to_pop = std::min(len, s);
    _tail = (_tail + to_pop) % _capacity;
    _full = false;
}

void CircularBuffer::append(const std::string& data)
{
    if (data.empty())
        return;

    if (data.size() > available_space()) {
        size_t new_capacity = _capacity * 2;
        while (new_capacity - size() < data.size()) {
            new_capacity *= 2;
        }
        resize(new_capacity);
    }

    size_t to_write = std::min(data.size(), available_space());

    for (size_t i = 0; i < to_write; ++i) {
        _buffer[_head] = data[i];
        _head = (_head + 1) % _capacity;
    }

    if (to_write > 0 && _head == _tail) {
        _full = true;
    }
}

bool CircularBuffer::extract_line(std::string& line)
{
    if (is_empty())
        return false;

    size_t current = _tail;
    size_t length = 0;
    bool found = false;

    while (length < size()) {
        if (_buffer[current] == '\n') {
            found = true;
            break;
        }
        current = (current + 1) % _capacity;
        length++;
    }

    if (!found)
        return false;

    line.clear();
    line.reserve(length);
    
    for (size_t i = 0; i < length; ++i) {
        line.push_back(_buffer[_tail]);
        _tail = (_tail + 1) % _capacity;
    }

    // Skip the '\n'
    _tail = (_tail + 1) % _capacity;
    _full = false;

    // Remove '\r' if present
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    return true;
}

bool CircularBuffer::is_empty() const
{
    return (!_full && (_head == _tail));
}

std::string CircularBuffer::peek_all() const
{
    if (is_empty())
        return "";

    size_t s = size();
    std::string result;
    result.reserve(s);

    size_t tmp_tail = _tail;
    for (size_t i = 0; i < s; ++i) {
        result.push_back(_buffer[tmp_tail]);
        tmp_tail = (tmp_tail + 1) % _capacity;
    }

    return result;
}
