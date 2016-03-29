Hello = Hello or {}
Hello.__index = Hello

Hello.name = "Diego"

function Hello:initialize(self, owner):
	print("Hello, " .. self.name)
end