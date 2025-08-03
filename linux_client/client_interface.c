#include "client_interface.h"

int main(){

    int sockfd;
    test_command_t cmd;
    test_result_t  result;
    struct sockaddr_in uut_addr;
    struct sockaddr_in from_addr;
    socklen_t from_len;


    srand(time(NULL));
    sockfd = socket(AF_INET,SOCK_DGRAM, 0);
    init_socket(&uut_addr, sockfd);
    ping_test(sockfd, cmd, uut_addr, &result, &from_addr, &from_len);
    interface( sockfd, &cmd, &uut_addr, &from_len, &from_addr, &result);
    close(sockfd);
    return 0;
}

