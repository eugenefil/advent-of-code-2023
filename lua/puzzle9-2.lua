require "utils"

local sum = 0
for line in io.lines() do
	local vals = string.split(line)
	for i in ipairs(vals) do vals[i] = vals[i] + 0 end

	local first = {}
	local nonzero = true
	while nonzero do
		local diffs = {}
		nonzero = false
		for i = 1, #vals - 1 do
			local diff = vals[i + 1] - vals[i]
			table.insert(diffs, diff)
			if diff ~= 0 then nonzero = true end
		end
		table.insert(first, vals[1])
		vals = diffs
	end

	local new = 0
	while first[1] do
		new = table.remove(first) - new
	end
	sum = sum + new
end
print(sum)
