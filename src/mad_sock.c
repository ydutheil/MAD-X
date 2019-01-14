#include "mad_sock.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

void py_coll_(char* el_name, double* theta, int* ktrack, double* track, int* part_id,
              int* n_lost, int* id_lost, double* s_lost)
{  
  struct sockaddr_in address;
  int socket_fd, size_msg;
    
  /* setting and openning socket */
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0){
    printf("socket() failed\n");
    return ;
  } 

  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_family = AF_INET;
  address.sin_port = htons( 50007 );

  if( connect(socket_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
    perror("connect failed");
    exit(-1);
  }
  

  size_msg = strlen(el_name);
  /* printf("el_name is length %d \n", size_msg); */
  if( send(socket_fd, el_name, size_msg, 0) < 0 ) {
    perror("sending el_name failed");
    return;
  }

  /* printf("theta is length %d \n", sizeof(*theta)); */
  if( send(socket_fd, theta, sizeof(*theta), 0) < 0 ) {
    perror("sending theta failed");
    return;
  }
  
  
  /* printf("ktrack is length %d \n", sizeof(*ktrack)); */
  if( send(socket_fd, ktrack, sizeof(*ktrack), 0) < 0 ) {
    perror("sending ktrack failed");
    return;
  }


  size_msg=((int) sizeof(*track))*6 *(*ktrack);
  /* printf("track size is %d \n", size_msg); */
  if( send(socket_fd, track, size_msg, 0) < 0 ) {
    perror("sending track failed");
    return;
  }

  size_msg=((int) sizeof(*part_id)) *(*ktrack);
  if( send(socket_fd, part_id, size_msg, 0) < 0 ) {
    perror("sending part_id failed");
    return;
  }
    
 
  if( recv_all(socket_fd, (char *)(&*n_lost), sizeof(*n_lost)) < 0 ) {
    perror("receiving n_lost failed");
    return;
  }

  if(*n_lost>0){
    int id_lost_loc[*n_lost];
    if( recv_all(socket_fd, (char *)(&id_lost_loc[0]), sizeof(id_lost_loc)) < 0 ) {
      perror("receiving id_lost failed");
      return;
    }
    /* for some reason filling the passed array id_lost was problematic, may no longuer be*/
    for(int n=0; n<*n_lost; ++n){
      id_lost[n] = id_lost_loc[n];
    }
    
    double s_lost_loc[*n_lost];
    if( recv_all(socket_fd, (char *)(&s_lost_loc[0]), sizeof(s_lost_loc)) < 0 ) {
      perror("receiving s_lost failed");
      return;
    }
    /* for some reason filling the passed array id_lost was problematic, may no longuer be*/
    for(int n=0; n<*n_lost; ++n){
      s_lost[n]=s_lost_loc[n];
    }
  }

  if( recv_all(socket_fd, (char *)(&track[0]), sizeof(track[0])*(*ktrack)*6) < 0 ) {
    perror("receiving track failed");
    return;
  }
  
  close(socket_fd);
  return;
}


/* Function to receive all expected data */
/* recv may receive data in some chunks, this is decided vy the network/sending system */
int recv_all(int sock, char *buf, unsigned int len)
{
    unsigned int n = 0;
    int status;
    while (n < len) {
      status = recv(sock, buf + n, len - n, 0);
        if (status == 0) {
            // Unexpected End of File
            return -1; // Or whatever
        } else if (status < 0) {
            // Error
            return -1; // Need to look at errno to find out what happened
        } else {
            n += status;
        }
     }
     return (int)n;
}
