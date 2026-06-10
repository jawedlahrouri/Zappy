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

        std::vector<char> m_buffer;
        size_t m_head;
        size_t m_tail;
        bool m_full;
        size_t m_capacity;
};
