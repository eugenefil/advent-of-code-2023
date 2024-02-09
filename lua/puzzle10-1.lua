require "utils"

local grid = {}
local y = 1
local S
for line in io.lines() do
	local row = {}
	for i = 1, #line do table.insert(row, string.sub(line, i, i)) end
	table.insert(grid, row)

	local x = string.find(line, "S")
	if x then S = {x=x, y=y} end
	y = y + 1
end
local h, w = #grid, #grid[1]
print("h", h, "w", w)
assert(S)
print("S", dump(S))

local xlconn = {"-", "L", "F"}
local xrconn = {"-", "J", "7"}
local ytconn = {"|", "7", "F"}
local ybconn = {"|", "L", "J"}
-- turn lists into searchable hashes
for _, conn in ipairs({xlconn, xrconn, ytconn, ybconn}) do
	for _ = 1, #conn do
		conn[table.remove(conn)] = 1
	end
	print(dump(conn))
end

local DP = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}
local ends = {}
local xconn = {[-1]=xlconn, [1]=xrconn}
local yconn = {[-1]=ytconn, [1]=ybconn}
for _, dp in ipairs(DP) do
	local dx, dy = unpack(dp)
	local x, y = S.x + dx, S.y + dy
	if x < 1 or x > w or y < 1 or y > h then
		-- out of grid
	else
		local tile = grid[y][x]
		if xconn[dx] and xconn[dx][tile] or
				yconn[dy] and yconn[dy][tile] then
			table.insert(ends, {x=x, y=y, x0=S.x, y0=S.y})
		end
	end
end
assert(#ends == 2)
print("ends", dump(ends))

local conn = {
	["|"]={x={}, y={[-1]=ytconn, [1]=ybconn}},
	["-"]={x={[-1]=xlconn, [1]=xrconn}, y={}},
	["L"]={x={[1]=xrconn}, y={[-1]=ytconn}},
	["J"]={x={[-1]=xlconn}, y={[-1]=ytconn}},
	["7"]={x={[-1]=xlconn}, y={[1]=ybconn}},
	["F"]={x={[1]=xrconn}, y={[1]=ybconn}},
}

local steps = 1
while not (ends[1].x == ends[2].x and ends[1].y == ends[2].y) do
	for _, e in ipairs(ends) do
		local found = false
		for _, dp in ipairs(DP) do
			local dx, dy = unpack(dp)
			local x, y = e.x + dx, e.y + dy
			if x < 1 or x > w or y < 1 or y > h then
				-- out of grid
			elseif x == e.x0 and y == e.y0 then
				-- prev position
			else
				local cur = grid[e.y][e.x]
				local nxt = grid[y][x]
				local xconn = conn[cur].x[dx]
				local yconn = conn[cur].y[dy]
				if xconn and xconn[nxt] or
						yconn and yconn[nxt] then
					e.x0, e.y0 = e.x, e.y
					e.x, e.y = x, y
					found = true
					break
				end
			end
		end
		assert(found)
	end
	steps = steps + 1
end
print(steps)
