shell:
	cc  -o $@ *.c

clean:
	rm shell

install:
	cp shell /bin/shell && ln -sf /sbin/shell
	echo "/bin/shell" >> /etc/shells
