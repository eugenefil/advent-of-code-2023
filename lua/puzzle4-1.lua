require "utils"

local sum = 0
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

	local points = 0
	for _, num in ipairs(ours) do
		if win_nums[num] then
			points = points == 0 and 1 or points * 2
		end
	end
	sum = sum + points
end
print(sum)
