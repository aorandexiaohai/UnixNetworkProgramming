gcc server_8_3.c ../Chapter3/read_write_helper.c -I.. -o x1
gcc client_8_7.c ../Chapter3/read_write_helper.c -I.. -o x2
gcc client_8_8.c ../Chapter3/read_write_helper.c ../Chapter3/sock_ntop.c -I.. -I../Chapter3 -o x3
