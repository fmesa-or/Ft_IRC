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
Look how to make handleJoin.
Complete tester with new parts added

# 2026/07/07
HandleJoin process the JOIN command to add a user to a channel. Checks for modes, passwords, limits and sends the exit to the rest of users in channel. ("Pepito has join the channel")
	-Reads parameters like channel name, key for channel
	-Validates client Nick & User
	-Searchs or creates the channel
	-Chechks restrictions like +i (InviteOnly), +k (key), +l (user limit)