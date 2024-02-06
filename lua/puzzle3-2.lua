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

	local p1, p2 = string.find(line, "[*]")
	while p1 do
		table.insert(schemline.syms,
			{string.sub(line, p1, p1), p1=p1, p2=p2})
		p1, p2 = string.find(line, "[*]", p2 + 1)
	end

	table.insert(schematic, schemline)
end

local sum = 0
for i, cur in ipairs(schematic) do
	for _, sym in ipairs(cur.syms) do
		sym.adj = {}
		sym.nadj = 0
		local neighbors = {cur}
		-- add line above and below, nils are effectively ignored
		table.insert(neighbors, schematic[i - 1])
		table.insert(neighbors, schematic[i + 1])
		for _, neigh in ipairs(neighbors) do
			for _, num in ipairs(neigh.nums) do
				if num.p1 > sym.p2 + 1 then break end
				if num.p2 < sym.p1 - 1 then
					-- continue
				else
					sym.nadj = sym.nadj + 1
					if sym.nadj > 2 then break end
					table.insert(sym.adj, num[1] + 0)
				end
			end
			if sym.nadj > 2 then break end
		end

		if sym.nadj == 2 then 
			sum = sum + sym.adj[1] * sym.adj[2]
		end
	end
end
print(sum)
