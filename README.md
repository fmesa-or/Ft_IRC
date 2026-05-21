<!--
DELETE THIS BEFORE SUBMIT

README REQUIREMENTS

A README.md file must be provided at the !!root¡¡ of your Git repository. Its purpose is to allow anyone unfamiliar with the project (peers, staff, recruiters, etc.) to quickly understand what the project is about, how to run it, and where to find more information on the topic.

The README.md must include at least:
	•The very first line must be italicized and read: This project has been created as part of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].

	•A “Description” section that clearly presents the project, including its goal and a brief overview.

	•An “Instructions” section containing any relevant information about compilation, installation, and/or execution.

	•A “Resources” section listing classic references related to the topic (documention, articles, tutorials, etc.), as well as a description of how AI was used — specifying for which tasks and which parts of the project.

	➠ Additional sections may be required depending on the project (e.g., usage examples, feature list, technical choices, etc.).

**Your README must be written in English.**
-->

<div align="center">

*This project has been created as part of the 42 curriculum by [**crmorale**](https://github.com/Crmorale) and [**fmesa-or**](https://github.com/fmesa-or)*

---

# FT_IRC

![C++](https://img.shields.io/badge/C%2B%2B-98-00599C?style=flat-square&logo=cplusplus) ![Makefile](https://img.shields.io/badge/Makefile-427819?style=flat-square) ![IRC](https://img.shields.io/badge/IRC-Server-blue?style=flat-square) ![Poll](https://img.shields.io/badge/Poll-Async-brightgreen?style=flat-square) ![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat-square&logo=linux&logoColor=black) ![42 School](https://img.shields.io/badge/42-School-000000?style=flat-square)

*A fully-featured IRC (Internet Relay Chat) server implementation in C++98*

</div>

---

## Description

**ft_irc** is a C++98 IRC server that allows multiple clients to connect and communicate in real-time via channels and private messages. It implements the core IRC protocol functionality with support for channel operators and advanced channel management features.

### Key Features

<!--
- **Multi-Client Support**: Handle multiple concurrent connections using `poll()`
- **Channel Management**: Create, join, and manage channels with topic and user limits
- **Operator Commands**: Full support for channel operator privileges (KICK, INVITE, TOPIC, MODE)
- **User Authentication**: Connection password protection
- **Robust Protocol Handling**: Proper packet aggregation and error handling
-->

### Project Constraints
- ✅ Written in **C++98 standard**
<!--
- ✅ Uses `poll()` for multiplexed I/O (no blocking, no forks)
-->
- ✅ Compliant with 42 Norminette
- ✅ No IRC client implementation
- ✅ No server-to-server communication

---

## Instructions

### Requirements
- **Compiler**: `c++`
- **Standard**: `-std=c++98`
- **Compilation Flags**: `-Wall -Wextra -Werror`
- **OS**: Linux

### Compilation

```bash
# Clone the repository
git clone git@github.com:fmesa-or/Ft_IRC.git
cd ft_irc

# Compile the project
make

# Clean build artifacts
make clean

# Remove all generated files
make fclean

# Rebuild everything
make re
```

### Execution

```bash
./ircserv <port> <password>
```

**Arguments:**
- `<port>`: The listening port for incoming IRC connections (e.g., 6667)
- `<password>`: The connection password required by IRC clients

**Example:**
```bash
./ircserv 6667 mySecretPassword
```

### Testing

Test the server using `nc` (netcat) to verify proper command processing:

```bash
$ nc -C 127.0.0.1 6667
com^Dman^Dd
$
```

Use `Ctrl+D` to send commands in separate packets (simulating low bandwidth or partial data).

---

## Implemented Commands

### User Commands
- `PASS` - Set connection password
- `NICK` - Set or change user nickname
- `USER` - Specify user information
- `JOIN` - Join a channel
- `PART` - Leave a channel
- `PRIVMSG` - Send private messages
- `QUIT` - Disconnect from server
- `[TODO]` - Other standard IRC commands to be implemented

### Channel Operator Commands
- `KICK` - Eject a client from the channel
- `INVITE` - Invite a client to a channel
- `TOPIC` - Change or view the channel topic
- `MODE` - Manage channel modes:
  - `i` - Invite-only channel
  - `t` - Restrict TOPIC command to operators
  - `k` - Channel key (password)
  - `o` - Give/revoke channel operator privilege
  - `l` - Set user limit on channel

---

## Project Structure

```
ft_irc/
├── docs/
│   ├── en.subject.ft_irc.pdf  # Project subject
│   └── guideResume.txt        # Quick reference guide
├── include/
│   └── [Header files]         # Project headers (to be created)
├── src/
│   └── [Source files]         # Implementation files (to be created)
├── Makefile                   # Build configuration
└── README.md                  # This file
```

---

## Resources & References

<!--
- [RFC 1459 - Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459)
- [IRC Numerics Reference](https://www.irchelp.org/protocol/numeric/)
- [Socket Programming Guide](https://beej.us/guide/bgnet/)
- [Poll() Manual](https://man7.org/linux/man-pages/man2/poll.2.html)
- [C++98 Standard Reference](https://en.cppreference.com/w/cpp)
- [Errno Manual](https://man7.org/linux/man-pages/man3/errno.3.html)
- [Sigaction Reference](https://man7.org/linux/man-pages/man2/sigaction.2.html)
-->

### AI Usage

- Used for refine README format.

---
<!--
## Development Roadmap

### Phase 1: Server Architecture
- [ ] Socket initialization and binding
- [ ] Poll-based event loop implementation
- [ ] Client connection handling
- [ ] Connection cleanup and error handling

### Phase 2: Protocol Implementation
- [ ] IRC message parsing and validation
- [ ] User authentication (PASS, NICK, USER commands)
- [ ] Connection establishment and welcome messages
- [ ] Proper IRC reply codes (RPL_*, ERR_*)

### Phase 3: Core Functionality
- [ ] Channel creation and management
- [ ] Message broadcasting (PRIVMSG to channels and users)
- [ ] User listing and channel listing (NAMES, LIST)
- [ ] Basic server responses

### Phase 4: Operator Commands
- [ ] KICK, INVITE commands implementation
- [ ] TOPIC command implementation
- [ ] MODE command with all channel modes (i, t, k, o, l)
- [ ] User privilege management

### Phase 5: Advanced Features
- [ ] Packet fragmentation handling
- [ ] Proper buffering for incomplete messages
- [ ] Error handling for edge cases
- [ ] Connection timeout management

### Phase 6: Testing & Refinement
- [ ] Netcat testing with partial data
- [ ] Low bandwidth simulation
- [ ] Norminette compliance check
- [ ] Memory leak detection
- [ ] Edge case validation
-->
---

<!--
## Notes for Development

- Remember to aggregate received packets to rebuild complete commands
- Handle commands that arrive in multiple parts (see Testing section)
- Implement proper error codes as per RFC 1459
- Use `poll()` instead of `select()`, `kqueue()`, or `epoll()` as specified
- No forking allowed - use single-threaded multiplexed I/O
---
-->

<div align="center">

Made with ☕ at **42 School**

</div>
