require "utils"

local sum = 0
for line in io.lines() do
	local game, sets = unpack(string.split(line, ":"))
	assert(game and sets)

	sets = string.strip(sets)
	max = {red=1, green=1, blue=1}
	for _, cubes in ipairs(string.split(sets, "[,;] ")) do
		local n, color = string.match(cubes, "(%d+) (%a+)")
		assert(n and color)
		n = n + 0
		if n > max[color] then max[color] = n end
	end

	sum = sum + max.red * max.green * max.blue
end
print(sum)
