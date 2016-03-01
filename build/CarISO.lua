CarISO = CarISO or {}
CarISO.properties = {}

CarISO.properties.x = 0
CarISO.properties.y = 0
CarISO.properties.speed = 45.0

CarISO.properties.controls = {
    goleft = Input.KEY_LEFT,
    goright = Input.KEY_RIGHT,
	jump = Input.KEY_SPACE
}

local pressing = false
local jumping = false

function CarISO:update(owner, dt)
    local spr = owner:get_component("AnimatedSprite")
	
    -- Isometric Position --
    local sx = CarISO.properties.x - CarISO.properties.y
    local sy = (CarISO.properties.x + CarISO.properties.y) / 2
    owner.position = Vector2(sx, sy)
    ------------------------
    	
    if Input.get_key(CarISO.properties.controls.goleft) then
        pressing = true
        spr:set_animation("turn_left")
        if CarISO.properties.x >= -30 then
          CarISO.properties.x = CarISO.properties.x - CarISO.properties.speed * dt
        end
    elseif Input.get_key(CarISO.properties.controls.goright) then
        pressing = true
        spr:set_animation("turn_right")
        if CarISO.properties.x < 4 then
          CarISO.properties.x = CarISO.properties.x + CarISO.properties.speed * dt
        end
    else
        if pressing then
            spr:set_animation("run")
            pressing = false
        end
    end
	
	if not jumping then
		if Input.get_key_down(CarISO.properties.controls.jump) then
			jumping = true
			spr:set_animation("jump")
		end
	end
	
	if jumping then
		if spr.current_frame >= #spr.current_animation.frames-1 then
			jumping = false
			spr:set_animation("run")
		end
	end
end