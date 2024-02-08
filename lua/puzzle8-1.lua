require "utils"

local path = io.read()
io.read()
print("path", path)

local nodes = {}
for line in io.lines() do
	local name, targ = unpack(string.split(line, " = "))
	assert(name and targ)
	targ = string.strip(targ, "()")
	local l, r = unpack(string.split(targ, ", "))
	assert(l and r)
	nodes[name] = {L=l, R=r}
end

local cur = "AAA"
local len = #path
local i = 0
while cur ~= "ZZZ" do
	local j = (i % len) + 1
	local dir = string.sub(path, j, j)
	cur = nodes[cur][dir]
	i = i + 1
end
print(i)
