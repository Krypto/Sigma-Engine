pipe_spawn = {}

pipe_spawn.enabled = true
pipe_spawn.time = 0
pipe_spawn.max_time = 2.0

function pipe_spawn:update(dt)
	if pipe_spawn.enabled then
		pipe_spawn.time = pipe_spawn.time + dt

		if pipe_spawn.time >= pipe_spawn.max_time then
			pipe_spawn.time = 0
			pipe_spawn.max_time = math.random(90, 250) / 100

			local ipipe = self.scene:get_inactive_node("pipe")
			local jpipe = self.scene:get_inactive_node("pipe2")
			self.scene:instantiate(ipipe, 400/60)
			self.scene:instantiate(jpipe, 400/60)
		end
	end
end

function pipe_spawn:on_message(msg)
	if msg.body == "game_over" then
		pipe_spawn.enabled = false
	end
end

return pipe_spawn