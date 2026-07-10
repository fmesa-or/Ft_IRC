Breakdown of What You Need To Build
1. Server Startup

This is the initialization phase.

Parse arguments

You’ll eventually receive:

./ircserv <port> <password>

You should validate:

argc
port validity
password presence

Example:

int port = parse_port(argv[1]);
std::string password = argv[2];
Create socket

You’ll use:

socket(AF_INET, SOCK_STREAM, 0)

This creates a TCP socket.

You need to understand:

file descriptor
IPv4 socket
TCP stream socket
Configure socket

Likely:

setsockopt()

for:

SO_REUSEADDR

Useful for avoiding restart issues.

Bind

Associate socket with port.

bind()
Listen

Start accepting connections.

listen()

At this point:

your server is waiting for clients
2. poll() Event Loop

This is the heart of your subsystem.

You’ll likely maintain:

std::vector<pollfd> pollfds;

Example:

pollfd pfd;
pfd.fd = _listen_fd;
pfd.events = POLLIN;
Main Loop

Conceptually:

while (true)
{
    poll(...);

    handle listening socket;
    handle client sockets;
}
Why poll?

Because you must handle:

multiple clients
single-threaded
non-blocking style

without one client freezing the server.

3. Accepting New Clients

When the listening socket becomes readable:

new connection available

Use:

accept()

After accept:

You get:

int client_fd;

Then:

create Client object
insert into _clients
add fd to pollfds

Example:

_clients.insert(std::make_pair(fd, Client(fd)));
4. Receiving Data

When a client fd becomes readable:

Use:

recv()

Example:

char buffer[BUFFER_SIZE];
ssize_t bytes = recv(fd, buffer, BUFFER_SIZE, 0);

Three cases:

bytes > 0

Normal data received.

Append to:

client.recvBuffer()
bytes == 0

Client disconnected cleanly.

Remove client.

bytes < 0

Error.

Possibly disconnect.

5. Reconstruct Complete IRC Lines

This is the trickiest part of your role.

TCP does not preserve messages.

IRC lines end with:

\r\n

But you may receive:

Example A

JOIN #42\r\n

in one recv.

Easy.

Example B

JO

then later:

IN #42\r\n

Still same command.

Example C

Multiple commands at once:

JOIN #42\r\nPING hello\r\n

in one recv.

You must reconstruct complete lines from arbitrary chunks.

Example approach:

Append all incoming bytes:

client.recvBuffer() += received_data;

Then repeatedly search for:

"\r\n"

If found:

Extract line.

Remove it from buffer.

Repeat.

Eventually:

std::string line;

gets produced.

That line gets passed to Person B’s parser.

For now you can just:

std::cout << line << std::endl;
6. Outgoing Message Buffering

This is the send side.

Other parts of the program will do:

client.queueMessage(msg);

That only appends to:

_send_buffer

No sending yet.

Example:

_send_buffer += msg;
7. Flushing Outgoing Messages

You are responsible for actual sending.

Use:

send()

or:

write()

Conceptually:

send(client.fd, client.sendBuffer(), ...)

Then remove sent bytes from buffer.

Important case:

You may only send part of the buffer.

Example:

Buffer contains:

100 bytes

send returns:

40

You must keep remaining:

60 bytes

in buffer.

This matters.

8. Disconnect Handling

Very important.

When client leaves:

You must clean up everything.

Remove from:

_clients
pollfds

Later, during integration:

remove from channels
notify others

This is probably best handled by:

Server::disconnectClient(fd)
9. Non-blocking Mode (Important)

Check whether subject requires it explicitly.

Even with poll(), many IRC implementations use:

fcntl(fd, F_SETFL, O_NONBLOCK)

for sockets.

Worth discussing with teammates.

Suggested Internal Functions

I’d structure Server.cpp roughly like this:

void Server::run();

void Server::initializeSocket();

void Server::acceptClient();

void Server::handleClientInput(int fd);

void Server::handleClientOutput(int fd);

void Server::disconnectClient(int fd);

void Server::flushClientMessages(Client& client);

void Server::extractLines(Client& client);

That’s a clean breakdown.

Practical Milestones For You
Milestone A

Server starts.

Listening socket opens.

Milestone B

Multiple clients can connect.

Test with:

nc localhost 6667
Milestone C

Receive messages from clients.

Print raw bytes.

Milestone D

Reconstruct full IRC lines.

Print complete commands.

Example:

JOIN #42
PRIVMSG #42 :hello
Milestone E

Outgoing buffering works.

You can manually queue messages to clients.

At that point, your subsystem is ready.

Person B plugs parser here:

line -> parse_command()

Person C plugs channel logic after commands execute.

That’s basically your entire domain. If you build this robustly, integration becomes dramatically easier for the team.
