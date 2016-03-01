move = move or {}

move.properties = {}
move.properties.speed = 160

local ps = -(16*144)/4

function move:update(owner, dt)
	local pos = owner.position
	pos = pos + Vector2(0, move.properties.speed) * dt
	owner.position = pos
	
	if owner.position.y > ps + 144 then
		owner.position = Vector2(owner.position.x, ps)
	end
end