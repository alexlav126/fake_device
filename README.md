# fake_device
It is a client-server application that simulates remote control of the device.<br>
The device is a universal multichannel multimeter.<br>
Server - an application that implements direct control of the device hardware.<br>
Сlient - a simple console application that implements server management.<br>
qml_client - GUI client.<br>
Client and server communicate using socket unix domain.<br>
Text protocol. Request and response - string ordered by CR.<br>
Command format:<br>
"command-name parameter1, parameter2, ... parameterN\n"<br>
Response format: "ok|fail, result1, result2, ... resultN\n"<br>
For example:<br>
request: "set_range channel0, range0".<br>
the answer is "ok, range0".<br>

# Build
run ./build.sh<br>
(tested on debian 10)<br>

# Run
From build directory run the files server, client or qml_client.<br>
The server can serve multiple clients.<br>

# issues
Exception handling is omitted.<br>
The console client application is quite simple and not all errors are handled. It can only transfer raw data.<br>
qml_client is a demo, without automatic polling. All requests are sent by pressing buttons.<br>
