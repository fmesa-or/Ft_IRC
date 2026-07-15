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
Find how to handle _topicRestricted. Is it mandatory?	
Build methods to operate with _topicRestricted			
Is it mandatory to have a topic in everychannel?		
How we deal with add new operators? Is allowed?			
Look how commands works. How we resolve +k +i ....		
Look for limitations for setters						

# 2026/07/15
Touched:
	┠─CommandDispatcher.cpp: 24 -> added:  && cmd.name!= "JOIN"
	┠─Channel.cpp: 188 -> commented
	┠─Server.cpp: 269 -> findChannel & addChannel


Issues detected:
	┠─ Register not working

TODO
	┠─ Implement TopicRestricted
	┠─ Implement Add/Revoke Operator
	┠─ Implement Activate/Deactivate Topic Restricted