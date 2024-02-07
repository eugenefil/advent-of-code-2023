require "utils"

local function parseline()
	local t = string.split(io.read())
	table.remove(t, 1) -- remove header
	for i in ipairs(t) do t[i] = t[i] + 0 end
	return t
end
local times, distances = parseline(), parseline()
print(dump(times))
print(dump(distances))

-- Total distance that boat travels is given by formula: d = (T - h) * h,
-- where d - distance, T - total race time, h - hold time. It's a downward
-- opening parabola with roots at h = 0 and h = T and maximum at H = T / 2.
-- To find hold time needed to reach a target distance D a quadratic
-- equation must be solved:
--
-- h^2 - T*h + D = 0
-- h1 = (T - sqrt(T^2 - 4*D)) / 2
-- h2 = (T + sqrt(T^2 - 4*D)) / 2
--
-- Since parabola opens downward, holds times in (h1, h2) provide for all
-- distances greater than target D.

local res = 1
for i, T in ipairs(times) do
	local D = distances[i]
	local h1 = (T - math.sqrt(T * T - 4 * D)) / 2
	h1 = math.ceil(h1) == h1 and h1 + 1 or math.ceil(h1) -- next whole
	assert(h1 < T / 2)
	local H = math.floor(T / 2) -- parabola max
	local n = (H - h1 + 1) * 2 -- use parabola symmetry
	if 2 * H == T then
		-- if H is whole (e.g. for T = 8), max was counted twice
		n = n - 1
	end
	res = res * n
end
print(res)
