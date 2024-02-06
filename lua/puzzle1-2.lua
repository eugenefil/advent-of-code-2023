local sum = 0
local digits = {one=1, two=2, three=3, four=4, five=5, six=6, seven=7, eight=8, nine=9}
for line in io.lines() do
	local p1 = string.find(line, "%d")
	local d1
	if p1 then d1 = string.sub(line, p1, p1) end
	for k, v in pairs(digits) do
		local p = string.find(line, k)
		if p then
			if not p1 or p < p1 then
				p1 = p
				d1 = v
			end
		end
	end
	assert(d1)

	local d2
	for i = #line, 1, -1 do
		if string.find(line, "%d", i) then
			d2 = string.sub(line, i, i)
			break
		end
		for k, v in pairs(digits) do
			local p = string.find(line, k, i)
			if p then
				d2 = v
				break
			end
		end
		if d2 then break end
	end
	assert(d2)

	sum = sum + (d1 .. d2)
end
print(sum)
