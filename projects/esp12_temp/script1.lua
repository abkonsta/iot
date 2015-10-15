require('ds18b20')
gpio0 = 1

wifi.setmode(wifi.STATION)
wifi.sta.config("Franklin", "captain janeway")
ds18b20.setup(gpio0)

tmr.alarm(1, 3000, 1, function ()
	t1=ds18b20.read()
	t1=ds18b20.read()
	
	-- print the temp to serial
	print(t1)
	
	-- report the temp to thingspeak
	-- conn = net.createConnection(net.TCP, 0) 
	-- conn:on("receive", function(conn, payload) print(payload) end) 
	-- conn:connect(80,'api.thingspeak.com') 
	-- conn:send("GET /update?key=F0R1AG5EURHBQTNJ&field1="..t1.." HTTP/1.1\r\n") 
	-- conn:send("Host: api.thingspeak.com\r\n") 
	-- conn:send("Accept: */*\r\n") 
	-- conn:send("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n")
	-- conn:send("\r\n")
end)
	


