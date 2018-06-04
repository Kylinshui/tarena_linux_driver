# tarena_linux_driver
pdf 加密
tar -zcvf - pdf | openssl des3 -salt -k myname | dd    of=pdf.tar.gz
解密
dd if=pdf.tar.gz | openssl  des3 -d -k myname | tar zxf -
密码:myname 
