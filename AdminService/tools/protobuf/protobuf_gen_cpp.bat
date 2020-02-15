del ..\..\code\EVA\server\server_share\pb\*.* /f /a /q
del ..\..\code\EVA\server\script\DataTable\proto\*.* /f /a /q

copy *.proto ..\..\code\EVA\server\script\DataTable\proto\
protoc-3.5.1 --cpp_out=../../code/EVA/server/server_share/pb define_pro.proto msg_client.proto

pause

