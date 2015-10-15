--
-- send a GET call to the given host at the given URL
-- http if secure=0, https if secure=1
--
-- host: hostname
-- url: request URL, with URL params, etc
-- onSuccess: callback if the HTTP GET succeeds
--
-- example:
-- httpGet("myhost.com", "/myurl", mycallback}
--

function httpGet(host, url, onSuccess)
	
	connout = nil
  connout = net.createConnection(net.TCP, 0)
 
  connout:on("receive", function(connout, payloadout)
    if (string.find(payloadout, "Status: 200 OK") ~= nil) then
    	if(onSuccess ~= nil) then
				onSuccess();
			end
		end
  end)

  connout:on("connection", function(connout, payloadout)
    connout:send("GET " .. url .. " HTTP/1.1\r\n"
    .. "Host: " .. host .. "\r\n"
    .. "Connection: close\r\n"
    .. "Accept: */*\r\n"
    .. "User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n"
    .. "\r\n")
  end)

  connout:on("disconnection", function(connout, payloadout)
    connout:close();
    collectgarbage();
  end)

  connout:connect(80, host)
end
