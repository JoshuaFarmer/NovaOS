simple 32 bit operating system. <br>
Todo: <br>
finish fat32 - DOING <br>
graphics mode text - DONE ? <br>
memory protection - GONNA DO SOON ? <br>

<h1>RANDOM STUFF</h1>
<p>
	so, im gonna start with the only thing im going to state, here<br>
	<h3>PRINTF FORMATTING</h3>
	the printf formatting is very simple in this implementation of<br>
	printf, only having<br>
s<br>
d<br>
c<br>
T<br>
	s,d,c are the standard functions, however we have this "T"<br>
	which is for setting the display colour.<br>
	it's very easy to use in my opinion,<br>
	for instance:
	print("%T9.Hello, World!");
	%T<colour_code (4 bit VGA text mode)><full_stop>
</p>