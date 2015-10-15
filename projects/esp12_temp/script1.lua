require('ds18b20')
require('httpget')

wifi.setmode(wifi.STATION)
wifi.sta.config("Franklin", "captain janeway")

gpio0 = 1
ds18b20.setup(gpio0)

tmr.alarm(1, 3000, 1, function ()
	t1 = ds18b20.read()
	t1 = ds18b20.read()
		
	-- print the temp to serial
	print(t1)

  -- post
	host = "api.thingspeak.com"
	url = "/update?key=F0R1AG5EURHBQTNJ&field1="..t1
	httpGet{host = host, url = url}
end)
	


