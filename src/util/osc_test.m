u = udp('localhost', '9000');
fopen(u);
for i = 1:10
    oscsend(u, 'test', 'i', 1234);
end
fclose(u);