require "utils"

local cards = {}
for line in io.lines() do
	local nums = string.split(line, ":")[2]
	assert(nums)
	local winning, ours = unpack(string.split(nums, "|"))
	assert(winning and ours)

	local win_nums = {}
	winning = string.split(string.strip(winning))
	for _, num in ipairs(winning) do
		win_nums[num] = 1
	end
	ours = string.split(string.strip(ours))

	local wins = 0
	for _, num in ipairs(ours) do
		if win_nums[num] then wins = wins + 1 end
	end
	table.insert(cards, {n=1, wins=wins})
end

local sum = 0
for i, card in ipairs(cards) do
	sum = sum + card.n
	for j = 1, card.wins do
		if cards[i + j] then
			cards[i + j].n = cards[i + j].n + card.n
		end
	end
end
print(sum)
