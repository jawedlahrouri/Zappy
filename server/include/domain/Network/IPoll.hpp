/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IPoll.hpp
*/

/**
 * @file IPoll.hpp
 * @brief I/O multiplexing abstraction used by the server's main loop.
 */

#ifndef IPOLL_HPP_
#define IPOLL_HPP_

#include <cstddef>

/**
 * @enum NetworkEvent
 * @brief The kinds of readiness a file descriptor can be watched for.
 */
enum class NetworkEvent {
    Input,  /**< Readiness to read (maps to POLLIN). */
    Output  /**< Readiness to write (maps to POLLOUT). */
};

/**
 * @interface IPoll
 * @brief Abstraction over a @c poll()-based I/O multiplexer.
 *
 * @par Design Pattern: Strategy / abstract interface (Dependency Inversion)
 * @c IPoll hides the system @c poll() behind a stable contract so the server
 * loop never touches @c <poll.h> directly. The concrete @ref Poll implementation
 * is injected into the @c Server at construction, which means the multiplexer
 * can be swapped or mocked in unit tests without changing any game logic.
 *
 * @par What I/O multiplexing solves
 * The server handles many clients (AI + GUI) plus its own listening socket on a
 * single thread. Instead of blocking on one socket at a time, it registers every
 * file descriptor (fd) here and asks the OS, with a single @ref runPoll call,
 * "which of these are ready?". That call sleeps until at least one fd is ready or
 * a timeout elapses, so the server spends zero CPU while idle yet reacts
 * immediately to any client.
 *
 * @par How the watch list is maintained
 * Implementations keep an internal, ordered list of watched fds:
 * - @ref addFd registers a new fd (the master socket at startup, then each
 *   accepted client). Index 0 is, by convention, the listening socket.
 * - @ref removeFd drops a fd when its client disconnects.
 * - @ref addEvents / @ref removeEvents toggle the events watched for an existing
 *   fd. This is how @c POLLOUT is requested *on demand*: a client fd is normally
 *   watched for @c Input only; @c Output is added when there is pending data to
 *   write and removed again once the write buffer is drained, so @ref runPoll
 *   does not busy-spin returning "writable" on idle sockets.
 *
 * @par Reading the result
 * After @ref runPoll returns, results are queried *by index* (not by fd):
 * @ref isReadable, @ref isWritable and @ref isError test the i-th entry, and
 * @ref getFd maps that index back to its fd. The server walks indices in reverse
 * (see @c Server::handlePollEvents) so removing a disconnected fd mid-iteration
 * never shifts an index it has yet to visit.
 *
 * @par Typical fd life cycle
 * @verbatim
 *   addFd(master, Input)                    // startup, index 0
 *     -> runPoll() reports master readable  // an incoming connection
 *        -> accept() + addFd(client, Input) // new client, index N
 *           -> runPoll() reports client readable   // recvData()
 *              -> server queues a reply + addEvents(client, Output)
 *                 -> runPoll() reports client writable // sendData()
 *                    -> buffer drained -> removeEvents(client, Output)
 *           -> client disconnects -> removeFd(client)
 * @endverbatim
 */
class IPoll {
    public:
        virtual ~IPoll() = default;

        /**
         * @brief Registers a new fd to be watched for @p event.
         * @param fd File descriptor to watch.
         * @param event Initial readiness to watch for (usually @c Input).
         */
        virtual void addFd(int fd, NetworkEvent event) = 0;

        /**
         * @brief Stops watching @p fd entirely and drops it from the list.
         * @param fd File descriptor to remove (e.g. on disconnect).
         */
        virtual void removeFd(int fd) = 0;

        /**
         * @brief Adds @p event to the set already watched for @p fd.
         * @param fd Previously registered file descriptor.
         * @param event Event to start watching (e.g. @c Output when data is pending).
         */
        virtual void addEvents(int fd, NetworkEvent event) = 0;

        /**
         * @brief Removes @p event from the set watched for @p fd.
         * @param fd Previously registered file descriptor.
         * @param event Event to stop watching (e.g. @c Output once fully sent).
         */
        virtual void removeEvents(int fd, NetworkEvent event) = 0;

        /**
         * @brief Blocks until a watched fd is ready or @p timeout elapses.
         * @param timeout Milliseconds to wait; @c -1 blocks indefinitely,
         *        @c 0 returns immediately (non-blocking poll).
         * @return Number of fds with pending events (@c 0 on timeout).
         * @throws ServerException If the underlying @c poll() fails fatally.
         */
        virtual int runPoll(int timeout = -1) = 0;

        /**
         * @brief Tells whether the fd at @p index became readable.
         * @param index Position in the watch list (0 = listening socket).
         * @return @c true if @c POLLIN is set for that entry.
         */
        virtual bool isReadable(size_t index) const = 0;

        /**
         * @brief Tells whether the fd at @p index became writable.
         * @param index Position in the watch list.
         * @return @c true if @c POLLOUT is set for that entry.
         */
        virtual bool isWritable(size_t index) const = 0;

        /**
         * @brief Tells whether the fd at @p index reported an error/hang-up.
         * @param index Position in the watch list.
         * @return @c true if @c POLLERR, @c POLLHUP or @c POLLNVAL is set.
         */
        virtual bool isError(size_t index) const = 0;

        /**
         * @brief Returns the fd stored at @p index.
         * @param index Position in the watch list.
         * @return The file descriptor, or @c -1 if @p index is out of range.
         */
        virtual int getFd(size_t index) const = 0;

        /**
         * @brief Returns the number of currently watched fds.
         * @return Size of the watch list.
         */
        virtual size_t getSize() const = 0;
};

#endif /* !IPOLL_HPP_ */
