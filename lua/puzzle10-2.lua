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
			table.insert(ends, {x=x, y=y, x0=S.x, y0=S.y,
				dx=dx, dy=dy})
		end
	end
end
assert(#ends == 2)
print("ends", dump(ends))

-- detect tile for S and replace S with it
local S_tile
if ends[1].dy == -1 and ends[2].dy == 1 then S_tile = "|"
elseif ends[1].dx == -1 and ends[2].dx == 1 then S_tile = "-"
elseif ends[1].dy == -1 and ends[2].dx == 1 then S_tile = "L"
elseif ends[1].dx == -1 and ends[2].dy == -1 then S_tile = "J"
elseif ends[1].dx == -1 and ends[2].dy == 1 then S_tile = "7"
elseif ends[1].dx == 1 and ends[2].dy == 1 then S_tile = "F"
end
ends[1].dx, ends[1].dy, ends[2].dx, ends[2].dy = nil, nil, nil, nil
assert(S_tile)
print("S tile", S_tile)
grid[S.y][S.x] = S_tile

-- find loop
local conn = {
	["|"]={x={}, y={[-1]=ytconn, [1]=ybconn}},
	["-"]={x={[-1]=xlconn, [1]=xrconn}, y={}},
	["L"]={x={[1]=xrconn}, y={[-1]=ytconn}},
	["J"]={x={[-1]=xlconn}, y={[-1]=ytconn}},
	["7"]={x={[-1]=xlconn}, y={[1]=ybconn}},
	["F"]={x={[1]=xrconn}, y={[1]=ybconn}},
}
local loop = {
	[S.x .. " " .. S.y]=1,
	[ends[1].x .. " " .. ends[1].y]=1,
	[ends[2].x .. " " .. ends[2].y]=1,
}
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
	loop[ends[1].x .. " " .. ends[1].y] = 1
	loop[ends[2].x .. " " .. ends[2].y] = 1
end

-- replace debris pipes (i.e. not belonging to loop) with dots
for y = 1, h do
	for x = 1, w do
		local key = x .. " " .. y
		if not loop[key] then grid[y][x] = "." end
	end
end

local f = io.open("input10-clean", "w")
for _, row in ipairs(grid) do
	f:write(table.concat(row), "\n")
end
f:close()

local lrconn = {}
for _, left in ipairs({"-", "L", "F"}) do
	for _, right in ipairs({"-", "J", "7"}) do
		lrconn[left .. right] = 1
	end
end
print("left-right connections", dump(lrconn))

-- insert vertical corridors after every column except last
local widegrid = {}
for y, row in ipairs(grid) do
	local widerow = {}
	for x = 1, w - 1 do
		table.insert(widerow, row[x])
		local pair = row[x] .. row[x + 1]
		local conn = lrconn[pair] and "-" or " "
		table.insert(widerow, conn)
	end
	table.insert(widerow, row[w])
	table.insert(widegrid, widerow)
end
w = 2 * w - 1
grid = widegrid

local tbconn = {}
for _, top in ipairs({"|", "7", "F"}) do
	for _, bot in ipairs({"|", "L", "J"}) do
		tbconn[top .. bot] = 1
	end
end
print("top-bottom connections", dump(tbconn))

-- insert horizontal corridors after every row except last
local tallgrid = {}
for y = 1, h - 1 do
	table.insert(tallgrid, grid[y])
	local newrow = {}
	for x = 1, w do
		local pair = grid[y][x] .. grid[y + 1][x]
		local conn = tbconn[pair] and "|" or " "
		table.insert(newrow, conn)
	end
	table.insert(tallgrid, newrow)
end
table.insert(tallgrid, grid[h])
h = 2 * h - 1
grid = tallgrid

local f = io.open("input10-corridors", "w")
for _, row in ipairs(grid) do
	f:write(table.concat(row), "\n")
end
f:close()

-- find all horizontal strips of dots and spaces
local strips = {}
for y, row in ipairs(grid) do
	local ystrips = {}
	local x = 1
	while x <= w do
		while x <= w and row[x] ~= "." and row[x] ~= " " do
			x = x + 1
		end
		if x > w then break end

		local x0 = x
		local dots = 0
		while x <= w and (row[x] == "." or row[x] == " ") do
			if row[x] == "." then dots = dots + 1 end
			x = x + 1
		end
		table.insert(ystrips, {x0=x0, x1=x - 1, dots=dots})
	end
	strips[y] = ystrips
end

-- all top and bottom strips are outer strips
for _, s in ipairs(strips[1]) do s.outer = 1 end
for _, s in ipairs(strips[h]) do s.outer = 1 end

-- edge-touching left and right strips are outer strips
for y = 2, h - 1 do
	local left = strips[y][1]
	if left.x0 == 1 then left.outer = 1 end
	local right = strips[y][#strips[y]]
	if right.x1 == w then right.outer = 1 end
end

local changed = true
while changed do
	changed = false
	-- propagate outer strips top down by finding adjacent strips
	for y = 2, h - 1 do
		local ystrips = strips[y]
		local tstrips = strips[y - 1]
		for _, s in ipairs(ystrips) do
			if not s.outer then
				for _, t in ipairs(tstrips) do
					if t.outer then
						if t.x1 < s.x0 then
							-- pass
						elseif t.x0 > s.x1 then
							break -- no adjacent
						else
							s.outer = 1
							changed = true
							break
						end
					end
				end
			end
		end
	end

	-- propagate bottom up
	for y = h - 1, 2, -1 do
		local ystrips = strips[y]
		local bstrips = strips[y + 1]
		for _, s in ipairs(ystrips) do
			if not s.outer then
				for _, b in ipairs(bstrips) do
					if b.outer then
						if b.x1 < s.x0 then
							-- pass
						elseif b.x0 > s.x1 then
							break -- no adjacent
						else
							s.outer = 1
							changed = true
							break
						end
					end
				end
			end
		end
	end
end

for y, ystrips in ipairs(strips) do
	for _, s in ipairs(ystrips) do
		if s.outer then
			for x = s.x0, s.x1 do
				grid[y][x] = "O"	
			end
		end
	end
end

local f = io.open("input10-outside", "w")
for _, row in ipairs(grid) do
	f:write(table.concat(row), "\n")
end
f:close()

local sum = 0
for y, ystrips in ipairs(strips) do
	for _, s in ipairs(ystrips) do
		if not s.outer then sum = sum + s.dots end
	end
end
print(sum)
