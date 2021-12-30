shell:
	cc  -o $@ *.c

clean:
	rm -f shell

install:
	cp shell /bin/shell && ln -sf /sbin/shell
	echo "/bin/shell" >> /etc/shells

remove:
	rm -f /bin/shell /sbin/shell
	- grep -v "/bin/shell" /etc/shells > /etc/shells.new
	- mv -f /etc/shells.new /etc/shells
	
update: remove shell install
