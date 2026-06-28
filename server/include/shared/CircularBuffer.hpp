/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CircularBuffer.hpp
*/

/**
 * @file CircularBuffer.hpp
 * @brief Growable ring buffer used to frame inbound client data into lines.
 */

#pragma once

#include <vector>
#include <string>

/**
 * @class CircularBuffer
 * @brief Ring buffer that accumulates bytes and extracts '\n'-terminated lines.
 *
 * Backs each client's read buffer: TCP delivers arbitrary chunks, so bytes are
 * appended here and complete protocol lines pulled out one at a time. The buffer
 * grows when needed and throws on overflow past its working capacity.
 */
class CircularBuffer {
    public:
        /**
         * @brief Builds a ring buffer with an initial capacity.
         * @param capacity Initial capacity in bytes.
         */
        explicit CircularBuffer(size_t capacity = 65536);
        ~CircularBuffer() = default;

        /**
         * @brief Appends bytes, growing the buffer if necessary.
         * @param data Bytes to append.
         * @throws std::overflow_error If the data cannot fit after growth.
         */
        void append(const std::string& data);
        /**
         * @brief Extracts the next complete '\n'-terminated line.
         * @param[out] line Receives the line (without the newline).
         * @return @c true if a full line was available.
         */
        bool extract_line(std::string& line);
        /**
         * @brief Discards the first @p len buffered bytes.
         * @param len Number of bytes to drop.
         */
        void pop(size_t len);

        /** @brief Tells whether the buffer holds no bytes. */
        bool is_empty() const;
        /** @brief Returns a copy of all buffered bytes without consuming them. */
        std::string peek_all() const;
        /** @brief Empties the buffer. */
        void clear();

    private:
        /** @brief Number of bytes currently stored. */
        size_t size() const;
        /** @brief Free space before a resize is required. */
        size_t available_space() const;
        /** @brief Grows the backing storage to @p new_capacity. */
        void resize(size_t new_capacity);

        std::vector<char> _buffer; /**< Backing storage. */
        size_t _head;              /**< Index of the oldest byte. */
        size_t _tail;              /**< Index past the newest byte. */
        bool _full;                /**< Distinguishes full from empty when head==tail. */
        size_t _capacity;          /**< Current capacity in bytes. */
};
