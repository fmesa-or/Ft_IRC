# 2026/06/22
Needed unitary tests to check:
	└─Channel:
		┠─Constructors	✅
		┠─getName		✅
		┠─addMember		✅
		┠─removeMember	✅
		┠─hasMember		✅
		┠─getMembers	✅
		┠─addOperator	✅
		┠─removeOperator✅
		┠─isOperator	✅
		┖─getOperators	✅

# 2026/06/23
Q: Can we have two channels with the same name?
Needed
	└─Channel
		┠─Setter&Getter for	
		┃	┠─ _key					✅
		┃	┠─ _invitedOnly			✅
		┃	┠─ _topicRestricted		✅
		┃	┖─ _userLimit			✅
		┖─R&W _invited container	✅

# 2026/06/24
Try to make a template for all the add, remove and has methods.	✅

# 2026/06/25
Needed description for templates.			✅
Should we give "const" to the new getters?
Look how to make handleJoin.				✅
Complete tester with new parts added		✅

# 2026/07/07
HandleJoin process the JOIN command to add a user to a channel. Checks for modes, passwords, limits and sends the exit to the rest of users in channel. ("Pepito has join the channel")
	-Reads parameters like channel name, key for channel
	-Validates client Nick & User
	-Searchs or creates the channel
	-Chechks restrictions like +i (InviteOnly), +k (key), +l (user limit)

# 2026/07/08
Channel suport added
	┠─ canJoin							✅
	┠─ handleJoin						✅
	┠─ handlePart						✅
	┠─ handleKick						✅
	┖─ handleInvite						✅
-Need to :
	┠─ Add descriptions to new methods	✅
	┠─ Add new methods to tester		✅
	┖─ Make unitary test				✅

# 2026/07/10
Look for the implementation of the tools made,
	something in betwen Cristian work and mine
	┠─ JOIN 											✅
Look for mode flags +o & +t								✅
	┠─ (+/-)o -> Gives/Revokes Operator rol
	┖─ (+/-)t -> Activates/Deactivates TopicRestricted
Find purpouse of _topicRestricted						✅
	┖─Only Operators can change Topic
Find how to handle _topicRestricted. Is it mandatory?	✅
Build methods to operate with _topicRestricted			✅
Is it mandatory to have a topic in everychannel?		✅ (No)
How we deal with add new operators? Is allowed?			✅ (+/-o)
Look how commands works. How we resolve +k +i ....		✅ (/mod)
Look for limitations for setters						✅

# 2026/07/15
Touched:
	┠─ CommandDispatcher.cpp: 24 -> added:  && cmd.name!= "JOIN"
	┠─ Channel.cpp: 188 -> commented
	┠─ Server.cpp: 269 -> findChannel & addChannel
	┠─ ProtocolHandlers.cpp: 20 -> handleCap
	┠─ ChannelHandlers.cpp: buildNamesList
	┖─ More...

Issues detected:
	┠─ Register in server not working
	┠─ We are "Joining" a channel the object, but we are not joined to hexchat ✅
	┠─ Second clients can not join channels already made but first client can
	┖─ Is the parser wrong? HexChannel says it must be "/join #PokeFans" but we are working without the '#' ('&' for local channels) UPDATE: FIXED just # ✅ // Need to fix &channel

TODO
	┠─ Implement TopicRestricted													
	┠─ Implement Add/Revoke Operator												
	┠─ Implement Activate/Deactivate Topic Restricted								
	┠─ Implement Topic creation with </topic #42 :Canal para el proyecto ft_irc>	
	┠─ Implement the JOIN for the HexChat so it will know we are in the channel		✅
		┠─ HexChat interaction ->	C: JOIN #PokeFans

									S: :nick!user@host JOIN #PokeFans
									S: :server 331 nick #PokeFans :No topic is set
									S: :server 353 nick = #PokeFans :@nick
									S: :server 366 nick #PokeFans :End of /NAMES list.

Notes:
	Modes should be changed wen an operator wants with /mod <channel> <flag> <parm if needed>

		MODE #42 +k hola		// Add password
		MODE #42 +k adios		// Change password
		MODE #42 -k				// Removes key

		MODE #42 +l 10			// Changes limit
		MODE #42 -l				// Removes limit

		MODE #42 +i				// Changes mode to invitation
		MODE #42 -i				// Setsoff invitation mode

		MODE #42 +t				// Activates Topic restricted
		MODE #42 -t				// Setsoff Topic Restricted
--------------------------------------------------------------------
		If a client uses TOPIC <channel> the topic must be shown
		How to look the list of operators? Is mandatory?

# 2026/07/16
