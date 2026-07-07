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

