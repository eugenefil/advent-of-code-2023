local sum = 0
for line in io.lines() do
	local d1 = assert(string.match(line, "%d"))
	local d2 = assert(string.match(string.reverse(line), "%d"))
	sum = sum + (d1 .. d2)
end
print(sum)
