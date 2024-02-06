require "utils"

local schematic = {}
for line in io.lines() do
	local schemline	= {syms={}, nums={}}
	local p1, p2 = string.find(line, "%d+")
	while p1 do
		table.insert(schemline.nums,
			{string.sub(line, p1, p2), p1=p1, p2=p2})
		p1, p2 = string.find(line, "%d+", p2 + 1)
	end

	local p1, p2 = string.find(line, "[^%d.]")
	while p1 do
		table.insert(schemline.syms,
			{string.sub(line, p1, p1), p1=p1, p2=p2})
		p1, p2 = string.find(line, "[^%d.]", p2 + 1)
	end

	table.insert(schematic, schemline)
end

local sum = 0
for i, cur in ipairs(schematic) do
	for _, num in ipairs(cur.nums) do
		local neighbors = {cur}
		-- add line above and below, nils are effectively ignored
		table.insert(neighbors, schematic[i - 1])
		table.insert(neighbors, schematic[i + 1])
		for _, neigh in ipairs(neighbors) do
			for _, sym in ipairs(neigh.syms) do
				if sym.p1 > num.p2 + 1 then break end
				if sym.p2 < num.p1 - 1 then
					-- continue
				else
					sum = sum + num[1]
				end
			end
		end
	end
end
print(sum)
