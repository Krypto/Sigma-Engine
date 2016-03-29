pipe_move = {}
pipe_move.speed = 60
pipe_move.on = true

function pipe_move:initialize()
	local r = self:get_component("RigidBody2D")
	if self.name == "pipe" then
		r.position = Vector2(180, math.random(40, 120))
	else
		r.position = Vector2(180, -math.random(40, 180))
	end
end

function pipe_move:update(dt)
	if pipe_move.on then
		local r = self:get_component("RigidBody2D")

		r.position = r.position - Vector2(pipe_move.speed * dt, 0)
	end
end

function pipe_move:on_message(msg)
	if msg.body == "game_over" then
		pipe_move.on = false
	end
end

return pipe_move