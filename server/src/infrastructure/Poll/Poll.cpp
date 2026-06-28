/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "Poll.hpp"
#include "ServerException.hpp"
#include <algorithm>

namespace {
    /**
     * @enum PollFlag
     * @brief POSIX @c poll() event bits, redeclared to avoid leaking <poll.h>.
     *
     * Values match the standard @c POLLIN/@c POLLOUT/... constants so the masks
     * can be tested directly against the @c revents returned by the backend.
     */
    enum PollFlag : short {
        PollIn        = 1,  /**< Data available to read (POLLIN). */
        PollOut       = 4,  /**< Writable without blocking (POLLOUT). */
        PollErr       = 8,  /**< Error condition (POLLERR). */
        PollHup       = 16, /**< Peer hung up (POLLHUP). */
        PollNval      = 32, /**< Invalid fd (POLLNVAL). */
        PollErrorMask = PollErr | PollHup | PollNval /**< Any error/hang-up. */
    };
}

/**
 * @brief Translates a @ref NetworkEvent into its raw @c poll() bit.
 * @param event High-level readiness kind.
 * @return The matching @ref PollFlag value, or @c 0 if unknown.
 */
static short eventToShort(NetworkEvent event)
{
    if (event == NetworkEvent::Input) {
        return PollIn;
    }
    if (event == NetworkEvent::Output) {
        return PollOut;
    }
    return 0;
}

Poll::Poll(const NetworkBackend::Symbols &symbols)
    : _symbols(symbols)
{
}

/**
 * @brief Appends a fresh entry watching @p fd for @p event, with no results yet.
 */
void Poll::addFd(int fd, NetworkEvent event)
{
    PollEntry entry{};
    entry.fd      = fd;
    entry.events  = eventToShort(event);
    entry.revents = 0;
    _pollfds.push_back(entry);
}

/**
 * @brief Erases every entry matching @p fd via the erase-remove idiom.
 * @note Safe to call for an unknown fd: it simply removes nothing.
 */
void Poll::removeFd(int fd)
{
    _pollfds.erase(
        std::remove_if(_pollfds.begin(), _pollfds.end(),
            [fd](const PollEntry &e) { return e.fd == fd; }),
        _pollfds.end());
}

/**
 * @brief OR-s @p event into the watched mask of @p fd (e.g. arm POLLOUT).
 * @details Leaves already-watched events untouched; does nothing if @p fd is
 *          not registered.
 */
void Poll::addEvents(int fd, NetworkEvent event)
{
    for (auto &entry : _pollfds) {
        if (entry.fd == fd) {
            entry.events |= eventToShort(event);
            break;
        }
    }
}

/**
 * @brief Clears @p event from the watched mask of @p fd (e.g. disarm POLLOUT).
 * @details Masks with the bitwise complement so other watched events survive;
 *          does nothing if @p fd is not registered.
 */
void Poll::removeEvents(int fd, NetworkEvent event)
{
    for (auto &entry : _pollfds) {
        if (entry.fd == fd) {
            entry.events &= static_cast<short>(~eventToShort(event));
            break;
        }
    }
}

/**
 * @brief Runs one @c poll() cycle over the whole watch list.
 *
 * @details Steps:
 * 1. Flatten @ref _pollfds into parallel @c fds / @c events scratch buffers
 *    (the backend takes separate arrays rather than a @c pollfd array).
 * 2. Call the backend @c poll(), which blocks until a fd is ready or
 *    @p timeout expires.
 * 3. On @c -1, treat an @c EINTR (interrupted by a signal) as "no events" and
 *    return @c 0 so the loop simply retries; any other failure is fatal.
 * 4. Copy the returned @c revents back into each entry so the @c isReadable /
 *    @c isWritable / @c isError queries can read them by index afterwards.
 *
 * @param timeout Milliseconds to wait (@c -1 = block forever, @c 0 = poll once).
 * @return Number of ready fds (@c 0 on timeout or @c EINTR).
 * @throws ServerException On a non-recoverable @c poll() error.
 */
int Poll::runPoll(int timeout)
{
    const unsigned int count = static_cast<unsigned int>(_pollfds.size());
    std::vector<int>   fds(count);
    std::vector<short> events(count);
    std::vector<short> revents(count, 0);

    for (unsigned int i = 0; i < count; ++i) {
        fds[i]    = _pollfds[i].fd;
        events[i] = _pollfds[i].events;
    }

    int ret = _symbols.poll(fds.data(), events.data(), revents.data(), count, timeout);

    if (ret == -1) {
        if (_symbols.isInterrupted()) {
            return 0;
        }
        throw ServerException(std::string("Poll failed: ") + _symbols.pollGetLastError());
    }

    for (unsigned int i = 0; i < count; ++i) {
        _pollfds[i].revents = revents[i];
    }
    return ret;
}

/**
 * @brief Bounds-checked test of the POLLIN bit on the last poll result.
 */
bool Poll::isReadable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollIn) != 0;
}

/**
 * @brief Bounds-checked test of the POLLOUT bit on the last poll result.
 */
bool Poll::isWritable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollOut) != 0;
}

/**
 * @brief Bounds-checked test of any error/hang-up bit on the last poll result.
 */
bool Poll::isError(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollErrorMask) != 0;
}

/**
 * @brief Maps a watch-list index back to its fd, or @c -1 if out of range.
 */
int Poll::getFd(size_t index) const
{
    if (index >= _pollfds.size()) {
        return -1;
    }
    return _pollfds[index].fd;
}

/**
 * @brief Returns the current number of watched fds.
 */
size_t Poll::getSize() const
{
    return _pollfds.size();
}
