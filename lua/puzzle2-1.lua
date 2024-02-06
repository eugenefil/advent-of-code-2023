require "utils"

local MAX = {red=12, green=13, blue=14}

local sum = 0
for line in io.lines() do
	local game, sets = unpack(string.split(line, ":"))
	assert(game and sets)

	local id = string.match(game, "Game (%d+)")
	assert(id)

	sets = string.strip(sets)
	for _, cubes in ipairs(string.split(sets, "[,;] ")) do
		local n, color = string.match(cubes, "(%d+) (%a+)")
		assert(n and color)
		if n + 0 > MAX[color] then
			id = 0
			break
		end
	end

	sum = sum + id
end
print(sum)
