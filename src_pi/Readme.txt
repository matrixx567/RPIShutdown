
http://blog.scphillips.com/2013/07/getting-a-python-script-to-run-in-the-background-as-a-service-on-boot/

add shutdown script to startup


sudo chmod 755 /etc/init.d/myservice.py
sudo update-rc.d myservice.sh defaults


sudo service myservice.sh start

/etc/init.d/myservice.sh status
ls -l /etc/rc?.d/*myservice.sh