require "utils"

local seeds = io.read()
assert(seeds)
seeds = string.split(seeds, ": ")[2]
assert(seeds)
seeds = string.split(string.strip(seeds))
for i in ipairs(seeds) do seeds[i] = seeds[i] + 0 end
local seedrngs = {}
for i = 1, #seeds, 2 do
	local start, len = seeds[i], seeds[i + 1]
	table.insert(seedrngs, {x1=start, x2=start + len - 1})
end
print('seedrngs', dump(seedrngs))
io.read()

local maps = {}
for line in io.lines() do
	local map = {}
	map.name = string.split(line)[1]
	assert(map.name)
	map.ranges = {}
	for rngline in io.lines() do
		if rngline == "" then break end
		local range = string.split(rngline)
		assert(#range == 3)
		local dst, src, len = range[1] + 0, range[2] + 0, range[3] + 0
		table.insert(map.ranges, {s1=src, s2=src + len - 1, d1=dst})
	end
	table.insert(maps, map)
end

for _, map in ipairs(maps) do
	local i = 1
	local seed = seedrngs[i]
	while seed do
		for _, range in ipairs(map.ranges) do
			if not (seed.x2 < range.s1 or seed.x1 > range.s2) then
				if seed.x1 < range.s1 then
					table.insert(seedrngs, {x1=seed.x1,
						x2=range.s1 - 1})
					seed.x1 = range.s1
				end

				if seed.x2 > range.s2 then
					table.insert(seedrngs, {x1=range.s2 + 1,
						x2=seed.x2})
					seed.x2 = range.s2
				end

				seed.x1 = seed.x1 - range.s1 + range.d1
				seed.x2 = seed.x2 - range.s1 + range.d1
				break
			end
		end
		i = i + 1
		seed = seedrngs[i]
	end
end

local min = seedrngs[1].x1
for _, seed in ipairs(seedrngs) do
	if seed.x1 < min then min = seed.x1 end
end
print(min)
