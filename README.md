## 1. 功能

server 循环接受 client 发来的文件，并能够读取其中的内容。但还无法并发，且读不到文件的名字

client 需要指定 server 的 IP，同时直接指定发送的文件，例如，在命令行中直接执行
`./FileTransCli-ai 127.0.0.1 ../111.txt`。