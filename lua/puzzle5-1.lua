require "utils"

local seeds = io.read()
assert(seeds)
seeds = string.split(seeds, ": ")[2]
assert(seeds)
seeds = string.split(string.strip(seeds))
for i in ipairs(seeds) do seeds[i] = seeds[i] + 0 end
print('seeds', dump(seeds))
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
		range.len = table.remove(range) + 0
		range.src = table.remove(range) + 0
		range.dst = table.remove(range) + 0
		table.insert(map.ranges, range)
	end
	table.insert(maps, map)
end

local locations = {}
for _, seed in ipairs(seeds) do
	for _, map in ipairs(maps) do
		for _, range in ipairs(map.ranges) do
			if range.src <= seed and seed < range.src + range.len then
				seed = seed - range.src + range.dst
				break
			end
		end
	end
	table.insert(locations, seed)
end

local min = locations[1]
for _, loc in ipairs(locations) do
	if loc < min then min = loc end
end
print(min)
