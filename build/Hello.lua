-- First we declare our component
local Test = {}

-- Now we add some functions to it. Please note that, if you 
-- want custom functions, you need to declare a 'properties'
-- table and then define them there.
--
-- The built-in functions are:
--		- initialize		 -- Called when the owner/self Node is added to the scene
--		- update			 -- Called every frame. Takes in the delta_time	
--		- on_collision_enter -- Called when a collideable Node collides with another one. Takes in the collision_info
--		- on_collision_exit  -- Called when a collideable Node stops colliding with another one. Takes in the collision_info
--		- on_message		 -- Called when some Node sends a message to this Node. Takes in the message_object
function Test:initialize()
	print("Hello, i'm being called from '" .. self.name .. "'")
end

-- Now, the most important part, return the component
-- so the engine can capture its contents when reading the 
-- script.
return Test