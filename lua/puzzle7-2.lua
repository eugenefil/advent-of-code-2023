require "utils"

local function get_type(hand)
	hand = {unpack(hand)}
	table.sort(hand)

	local groups = {}
	local cur = table.remove(hand)
	assert(cur)
	local n, max, imax = 1, 1, 1
	local len = 0
	local njokers = 0
	while true do
		local c = table.remove(hand)
		if not c or c ~= cur then
			if cur == 'J' then
				njokers = n
			else
				table.insert(groups, n)
				len = len + 1
				if n > max then
					max = n
					imax = len
				end
			end
			if not c then break end
			cur = c
			n = 1
		else
			n = n + 1
		end
	end
	if njokers > 0 then
		if njokers == 5 then
			len = 1
		else
			groups[imax] = groups[imax] + njokers
		end
	end

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

local labels = {"J", "2", "3", "4", "5", "6", "7", "8", "9", "T", "Q", "K", "A"}
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
