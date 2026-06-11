/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CircularBuffer.hpp
*/

#pragma once

#include <vector>
#include <string>

class CircularBuffer {
    public:
        explicit CircularBuffer(size_t capacity = 65536);
        ~CircularBuffer() = default;

        void append(const std::string& data);
        bool extract_line(std::string& line);
        void pop(size_t len);
        
        bool is_empty() const;
        std::string peek_all() const;
        void clear();

    private:
        size_t size() const;
        size_t available_space() const;
        void resize(size_t new_capacity);

        std::vector<char> _buffer;
        size_t _head;
        size_t _tail;
        bool _full;
        size_t _capacity;
};
