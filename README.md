Arduino Wake on LAN repeater
============================
This program is designed to build a wake on LAN magic packet forwarder, allowing everyone to wake computers across the Internet (this is called wake on WAN).

It was wrote and tested with an Arduino Uno + Ethernet shield.

To use it, you must configure your router to forward magic packets (UDP 9 or UDP 7 ports) to the Arduino, which will send it to every computer belonging to your LAN, making wake on LAN working via the internet.

IP change notifier
------------------
In addition, this project is shipped with another useful feature to log the Arduino public IP address (using a PHP script hosted on the internet) so you can always know what is your IP address without using any dynamic DNS service like DynDNS or no-ip.com or TZO.

Send magic packet
-----------------
You can use this website to send magic packet without installing any software : http://www.depicus.com/wake-on-lan/woli.aspx.