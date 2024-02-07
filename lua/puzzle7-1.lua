require "utils"

local function get_type(hand)
	hand = {unpack(hand)}
	table.sort(hand)

	local groups = {}
	local cur = table.remove(hand)
	local n = 1
	while hand[1] do
		local c = table.remove(hand)
		if c == cur then
			n = n + 1
		else
			table.insert(groups, n)
			cur = c
			n = 1
		end
	end
	table.insert(groups, n)

	local len = #groups
	if len == 1 then
		return 7 -- 5 of a kind
	elseif len == 2 then
		if groups[1] == 4 or groups[2] == 4 then
			return 6 -- 4 of a kind
		end
		return 5 -- full house
	elseif len == 3 then
		if groups[1] == 2 or groups[2] == 2 then
			return 3 -- 2 pair
		end
		return 4 -- 3 of a kind
	elseif len == 4 then
		return 2 -- 1 pair
	else
		return 1 -- high card
	end
end

local labels = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"}
local strength = {}
for i, v in ipairs(labels) do
	strength[v] = i
end

local function cmp(hand1, hand2)
	if hand1 == hand2 then return false end -- same hand
	if hand1._type == hand2._type then
		local c1, c2 = hand1.cards, hand2.cards
		for i = 1, #c1 do
			local s1, s2 = strength[c1[i]], strength[c2[i]]
			if s1 ~= s2 then return s1 < s2 end
		end
		assert(false) -- duplicates
	else
		return hand1._type < hand2._type
	end
end

local hands = {}
for line in io.lines() do
	local chars, bid = unpack(string.split(line))
	assert(chars, bid)
	local cards = {}
	for i = 1, #chars do
		table.insert(cards, string.sub(chars, i, i))
	end
	hand = {cards=cards, bid=bid + 0, _type=get_type(cards)}
	table.insert(hands, hand)
end

table.sort(hands, cmp)
local sum = 0
for i, hand in ipairs(hands) do
	sum = sum + i * hand.bid
end
print(sum)
