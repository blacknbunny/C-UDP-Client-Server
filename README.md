# C-UDP-Client-Server
C UDP Protocol Client And Server

## Compiling UDP Server

``` gcc udpserver.c -o udpserver ```


## Compiling UDP Client

``` gcc udpclient.c -o udpclient ```

------

## Usage :

Type | Usage
------------ | -------------
UDP Server | ./udpserver [port]
UDP Client | ./udpclient [local_ip] [local_or_remote_port]

------

## This If Structure Used for 'Bind: Address already in use' Error.

```c
if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
   perror("Setsockopt ");
   exit(1);
}
```
