Readme

cmds to Run : 

  To run the client : 
  
      g++ -I/usr/include/openssl/ client.cpp -lcrypto -Wno-deprecated -Wno-deprecated-declarations -o client
  To run the Server : 
  
      g++  server.cpp -lpthread -o server
