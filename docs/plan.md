# ft_irc Work Division

## Overview

The project will be divided by **subsystems**, not by IRC commands.

This approach minimizes integration conflicts and allows each team member to work largely independently while still understanding the entire codebase.

The architecture is based on the following core components:

* `Server`
* `Client`
* `Channel`
* `Command`
* `parse_command()`

### Ownership Rules

* `Server` owns all `Client` objects.
* `Server` owns all `Channel` objects.
* `Channel` stores `Client*` references but never owns clients.
* All networking operations belong exclusively to `Server`.
* `Client` stores IRC state and message buffers.
* `Command` is a plain struct.
* `parse_command()` is a free function.
* Command handlers implement IRC behavior.

---

# Shared Responsibilities

## Architecture

Before implementation begins, the team agrees on:

* Class interfaces
* Ownership rules
* Naming conventions
* Repository structure
* Coding style

The agreed architecture contract must be committed before subsystem work starts.

---

## Code Reviews

Every member should:

* Review code from other subsystems
* Understand the overall architecture
* Be capable of explaining any major component during evaluation

No subsystem should become a black box owned by a single person.

---

## Integration

All members participate in:

* Merging subsystems
* Debugging integration issues
* Testing with real IRC clients
* Preparing for evaluation

---

# Person A — Networking & Server Core

## Responsibilities

Own everything related to:

* TCP sockets
* Server startup
* Poll loop
* Client lifecycle
* Receiving data
* Sending data
* Disconnect handling

## Files

```text
Server.hpp
Server.cpp
main.cpp
```

## Tasks

### Server Initialization

Implement:

* `socket()`
* `bind()`
* `listen()`

### Event Loop

Implement:

* `poll()`
* Main server loop

### Client Acceptance

Implement:

* `accept()`

Create and register new `Client` objects.

### Receiving Data

Implement:

* `recv()`

Append incoming data to:

```cpp
client.recvBuffer()
```

### Message Reconstruction

Handle partial TCP packets.

Convert fragmented input such as:

```text
JO
IN #42\r
\n
```

into:

```text
JOIN #42
```

before passing it to the parser.

### Sending Data

Implement:

```cpp
flushClientMessages(Client&)
```

using:

```cpp
client.sendBuffer()
```

### Disconnect Handling

Remove disconnected clients from:

* Poll structures
* Channels
* Client map

---

# Person B — Protocol & Parsing

## Responsibilities

Own everything related to:

* IRC syntax
* Parsing
* Registration
* Command dispatching
* Protocol validation

## Files

```text
Command.hpp
Parser.cpp
Dispatcher.cpp
handlers/
```

## Tasks

### Command Representation

Implement:

```cpp
struct Command
```

### Parser

Implement:

```cpp
parse_command()
```

Example:

```text
PRIVMSG #42 :hello world
```

becomes:

```cpp
Command
{
    name = "PRIVMSG",
    params = ...
};
```

### Registration

Implement:

* `PASS`
* `NICK`
* `USER`

Manage:

```cpp
_has_password
_has_nickname
_has_username
```

### Command Dispatch

Implement dispatching logic:

```cpp
if (cmd.name == ...)
```

### Validation

Handle:

* Unknown commands
* Missing parameters
* Registration requirements

### Initial Handlers

Implement:

* `PASS`
* `NICK`
* `USER`
* `PING`
* `QUIT`

These can be implemented before channel support exists.

---

# Person C — Channels & IRC State

## Responsibilities

Own everything related to:

* Channels
* Membership
* Operators
* Modes
* Channel state

## Files

```text
Channel.hpp
Channel.cpp
```

## Tasks

### Membership Management

Implement:

```cpp
addMember()
removeMember()
hasMember()
getMembers()
```

### Operator Management

Implement:

```cpp
addOperator()
removeOperator()
isOperator()
getOperators()
```

### Channel Creation Support

Support channel creation through:

```cpp
handleJoin()
```

when a requested channel does not exist.

### Mode Representation

Prepare internal state for:

* `+i`
* `+t`
* `+k`
* `+o`
* `+l`

The full handlers can be implemented later.

### Membership Logic

Design and implement support for:

* `JOIN`
* `PART`
* `KICK`
* `INVITE`

from a channel-state perspective.

---

# Milestone 1 — Registration

## Goal

A real IRC client can connect and register.

### Features

* PASS
* NICK
* USER

### Requirements

* Networking operational
* Parsing operational
* Registration operational

No channels required.

No messaging required.

---

# Milestone 2 — Basic IRC Functionality

## Goal

Users can join channels and exchange messages.

### Features

* JOIN
* PART
* PRIVMSG

### Requirements

* Channel creation
* Membership management
* Message broadcasting

At this point the server should be genuinely usable.

---

# Milestone 3 — Subject Requirements

## Goal

Complete the mandatory IRC feature set.

### Features

* KICK
* INVITE
* TOPIC
* MODE

### Required Modes

* `i`
* `t`
* `k`
* `o`
* `l`

---

# Testing Phase

All team members participate.

## Networking Tests

* Multiple simultaneous clients
* Client disconnects
* Reconnects
* Partial packets
* Invalid input

## Registration Tests

* Wrong password
* Missing commands
* Duplicate nicknames

## Channel Tests

* JOIN
* PART
* KICK
* INVITE
* TOPIC
* MODE

## Messaging Tests

* Channel messages
* Private messages
* Multiple users in channels

---

# Final Notes

Subsystem ownership indicates the primary implementer, not exclusive ownership.

Every team member should understand:

* Networking flow
* Parsing flow
* Client lifecycle
* Channel management

The objective is parallel development with shared understanding of the complete system.
