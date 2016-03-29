bird = {}
bird.dead = false
bird.yv = -16

function bird:initialize()
	local r = self:get_component("RigidBody2D")
	r.type = 2
end

function bird:update(dt)
	local r = self:get_component("RigidBody2D")

	if not bird.dead then
		if Input.get_mouse_button_down(Input.LEFT_MOUSE) then
			local F = bird.yv + self.scene.gravity.y
			r:apply_impulse(Vector2(0, F), Vector2(0))
			
			local src = self:get_component("AudioSource")
			local h = src:play("flap")
			h.volume = 0.45
		end
		local vel = math.rad(-r.linear_velocity.y * 8)
		r.rotation = vel
	end
	
	local miny = -(App.window.height/2 + 20)
	if self.position.y <= miny then
		self:send_message("", "game_over", 0, 0)
		bird.dead = true
		
		local src = self:get_component("AudioSource")
		local h = src:play("hit")
		h.volume = 0.25
	end
end

function bird:on_collision_enter(col)
	if not bird.dead then
		local hit = col.hit_node
		local r = self:get_component("RigidBody2D")
		if hit.tag == "pipe" then
			local spr = self:get_component("AnimatedSprite")
			spr.color = Color(1.0, 0.0, 0.0, 1.0)
			
			self:send_message("", "game_over", 0, 0)
			
			local dir = hit.position - r.position
			dir = dir:normalised()
			
			r:apply_impulse(dir * bird.yv/2, Vector2(0))
			
			bird.dead = true
			
			local src = self:get_component("AudioSource")
			local h = src:play("hit")
			h.volume = 0.5
			
		elseif hit.tag == "floor" then
			local spr = self:get_component("AnimatedSprite")
			spr.color = Color(1.0, 0.0, 0.0, 1.0)
			
			self:send_message("", "game_over", 0, 0)
			
			bird.dead = true
			r.linear_velocity = Vector2(0)
		end
	end
end

return bird