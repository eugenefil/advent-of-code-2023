function dump(o)
	if type(o) == "number" then
		return tostring(o)
	elseif type(o) == "string" then
		return string.format("%q", o)
	elseif type(o) == "table" then
		local t = {"{"}
		local idxs = {}
		local extra = false
		for i, v in ipairs(o) do
			table.insert(t, dump(v))
			table.insert(t, ", ")
			idxs[i] = 1
			extra = true
		end

		for k, v in pairs(o) do
			if not idxs[k] then
				table.insert(t, string.format("%s=%s",
					dump(k), dump(v)))
				table.insert(t, ", ")
				extra = true
			end
		end
		if extra then table.remove(t) end

		table.insert(t, "}")
		return table.concat(t)
	else
		error("not implemented")
	end
end
--[[
local tests = {
	5,
	"abc",
	"",
	"I'm ok",
	'The "Beast"',
	{},
	{1, 2, 3},
	{foo='bar', bar='bee'},
	{foo='bar', 7, bar='bee', 3},
	{7, key={foo='bar', 5}, {baz='ben', 'abc'}, 100},
}
for _, v in ipairs(tests) do
	print(dump(v))
end
--]]

function string.split(s, sep)
	if sep == "" then error("empty separator") end
	local res = {}
	local i = 1
	local p1, p2 = string.find(s, sep, i)
	while p1 do
		table.insert(res, string.sub(s, i, p1 - 1))
		i = p2 + 1
		p1, p2 = string.find(s, sep, i)
	end
	table.insert(res, string.sub(s, i))
	return res
end
--[[
local tests = {
	{"", " "},
	{"abc", " "},
	{":abc", ":"},
	{"abc:", ":"},
	{":abc:def:ghi:", ":"},
}
for _, v in ipairs(tests) do
	local s, sep = unpack(v)
	print(dump(v), dump(string.split(s, sep)))
end
--]]

function string.lstrip(s, chars)
	chars = chars or "%s"
	if chars == "" then return s end
	return string.gsub(s, "^[" .. chars .. "]+", "")
end
--[[
local tests = {
	{"abc", ""},
	{" \n\tabc"},
	{"abc", "ba"},
}
for _, v in ipairs(tests) do
	local s, chars = unpack(v)
	print(dump(v), dump(string.lstrip(s, unpack({chars}))))
end
--]]

function string.rstrip(s, chars)
	chars = chars or "%s"
	if chars == "" then return s end
	return string.gsub(s, "[" .. chars .. "]+$", "")
end
--[[
local tests = {
	{"abc", ""},
	{"abc \n\t"},
	{"abc", "cb"},
}
for _, v in ipairs(tests) do
	local s, chars = unpack(v)
	print(dump(v), dump(string.rstrip(s, unpack({chars}))))
end
--]]

function string.strip(s, chars)
	chars = chars or "%s"
	if chars == "" then return s end
	return string.rstrip(string.lstrip(s, chars), chars)
end
--[[
local tests = {
	{"abc", ""},
	{"\t\n abc \n\t"},
	{"abcfoocba", "cab"},
}
for _, v in ipairs(tests) do
	local s, chars = unpack(v)
	print(dump(v), dump(string.strip(s, unpack({chars}))))
end
--]]
