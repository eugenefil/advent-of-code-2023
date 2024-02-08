require "utils"

local path = io.read()
io.read()
print("path", path)

local nodes = {}
local starts = {}
for line in io.lines() do
	local name, targ = unpack(string.split(line, " = "))
	assert(name and targ)
	assert(#name == 3)
	targ = string.strip(targ, "()")
	local l, r = unpack(string.split(targ, ", "))
	assert(l and r)
	nodes[name] = {L=l, R=r}

	if string.sub(name, 3) == "A" then
		table.insert(starts, name)
	end
end
print("starts", dump(starts))

local len = #path
local function count_steps(start)
	local i = 0
	while string.sub(start, 3) ~= "Z" do
		local j = (i % len) + 1
		local dir = string.sub(path, j, j)
		start = nodes[start][dir]
		i = i + 1
	end
	return i
end

local steps = {}
for _, start in ipairs(starts) do
	table.insert(steps, count_steps(start))
end
print("steps", dump(steps))
print(lcm(steps))
