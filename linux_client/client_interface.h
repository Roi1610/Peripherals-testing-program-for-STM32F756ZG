#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/time.h>
#include "protocol.h"

#define SERVER_PORT 9000
#define UUT_IP "10.100.102.100" 
#define LOG_FILE "peripheral_test_log.txt"

/**
 * @brief write log result to file
 * @param test_id - identifies the test
 * @param start_time - date and time the test was sent
 * @param duration - test length measured in seconds
 * @param result - success or failure
 */
void log_result(uint32_t test_id, time_t start_time, double duration, int result){
    FILE *log = fopen(LOG_FILE, "a");
    if(!log){
        perror("Failed to open file");
        return;
    }

    struct tm *log_time = localtime(&start_time);
    if (!log_time ){
        perror("Failed to get local time");
        fclose(log);
        return;
    }
    
    fprintf(log,
        "Test ID: %u\n"
        "Date & Time test sent: %02d-%02d-%04d %02d:%02d:%02d \n"
        "Test Duration: %.3f seconds\n"
        "Result: %s\n\n\n",
        test_id,
        log_time ->tm_mday, log_time ->tm_mon+1,log_time ->tm_year+1900,
        log_time ->tm_hour, log_time ->tm_min, log_time ->tm_sec,
        duration,
        result ? "Seccess" : "Failure"
    );

    fclose(log);
}


/**
 * @brief User menu
 * prints the user menu to the terminal
 */
void menu(){
    printf("\n****** UUT peripheral test menu ******\n");
    printf("1. Test TIMER\n");
    printf("2. Test UART\n");
    printf("3. Test SPI\n");
    printf("4. Test I2C\n");
    printf("5. Test ADC\n");
    printf("0. Exit\n");
    printf("**************************************\n");

    printf("Please enter your choice: \n");
}

/**
 * @brief create a udp socket and set upp address structure
 */
void init_socket(struct sockaddr_in *uut_addr, int sockfd){
    if (sockfd < 0){
        perror("Failed to open socket");
        exit(EXIT_FAILURE);
    }

    memset(uut_addr, 0, sizeof(*uut_addr));
    uut_addr->sin_family = AF_INET;
    uut_addr->sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, UUT_IP, &uut_addr->sin_addr) <= 0)
    {
        perror("Invaild uut address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief set timeout for ping test
 */
void set_timeout(struct timeval timeout, int sockfd){
      setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));   
}


/**
 * @brief send "ping" test to server
 * only if this ping test is successful
 * you can continue testing
 */
void ping_test(int sockfd, test_command_t cmd, struct sockaddr_in uut_addr,test_result_t  *result, struct sockaddr_in *from_addr, socklen_t *from_len){
    struct timeval ping_timeout = {5, 0}; 
    struct timeval no_timeout = {0};

    set_timeout(ping_timeout, sockfd);

    memset(&cmd, 0, sizeof(cmd));
    cmd.test_id = rand();
    cmd.peripheral = PING;  
    cmd.iterations = 0;
    cmd.pattern_len = 0;

    printf("Pinging UUT to verify connectivity...\n");

    ssize_t sent = sendto(sockfd, &cmd, sizeof(cmd), 0, (struct sockaddr*)&uut_addr, sizeof(uut_addr));
    if (sent < 0) {
        perror("Ping sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    ssize_t received = recvfrom(sockfd, result, sizeof(*result), 0, (struct sockaddr*)&from_addr, from_len);

    if (received < 0) {
        fprintf(stderr, "UUT did't respond to ping — aborting.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UUT is online. Proceeding with tests...\n");

    set_timeout(no_timeout, sockfd);
}

void clear_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}




/**
 * @brief this is the main function to send and recive data
 * @param sockfd - socket descriptor
 * @param cmd - pointer to test_command_t struct
 * @param uut_addr - pointer to sockaddr_in struct for uut 
 * @param from_len - represents the senders packet length
 * @param from_addr - pointer to sockaddr_in struct for pc client
 * @param result - pointer to test_result_t struct,
 */
void interface( int sockfd, test_command_t *cmd, struct sockaddr_in *uut_addr, socklen_t *from_len, struct sockaddr_in *from_addr, test_result_t  *result){
    while(1){
        menu();
        int choice;
        scanf("%d", &choice);
        clear_buffer();

        if (choice == 0){
            printf("\nExiting and closeing client....\n");
            close(sockfd);
            break;
        }

        uint8_t peripheral = 0;
        switch (choice){
        case 1: peripheral = PERIPHERAL_TIMER; break;
        case 2: peripheral = PERIPHERAL_UART; break;
        case 3: peripheral = PERIPHERAL_SPI; break;
        case 4: peripheral = PERIPHERAL_I2C; break;
        case 5: peripheral = PERIPHERAL_ADC; break;
        default:
            printf("Invaild choice, try again\n");
            continue;
        }

        memset(cmd, 0, sizeof(*cmd));
        cmd->test_id = rand();
        cmd->peripheral = peripheral;

        int temp = 0;
        printf("Enter number of iterations (max 255): ");
        scanf("%d",&temp);
        while((temp > 255) || (temp == 0))
        {
            printf("Invaild choice, try again\n");
            clear_buffer();
            scanf("%d",&temp);
        }
        cmd->iterations = (unsigned char)temp;

        getchar();
        printf("\n");

        switch (peripheral)
        {
        case PERIPHERAL_TIMER: memset(cmd->pattern , 0, MAX_PATTERN_LEN); break;
        case PERIPHERAL_ADC: memset(cmd->pattern , 0, MAX_PATTERN_LEN); break;
        
        default:
            printf("Enter bit pattern string (max 255 chars): ");
            fgets((char *)cmd->pattern, MAX_PATTERN_LEN, stdin);
            cmd->pattern_len = strlen((char *)cmd->pattern);
            if (cmd->pattern[cmd->pattern_len -1] == '\n'){
                cmd->pattern[--cmd->pattern_len] = '\0';
            }
            else {
                printf("Warning: input too long, extra characters discarded!\n");
                clear_buffer();
            }
            if (cmd->pattern_len < MAX_PATTERN_LEN){
                memset(cmd->pattern + cmd->pattern_len, 0, MAX_PATTERN_LEN -cmd->pattern_len);
            }
            break;
        }


        printf("\nPlease wait while the test is in progress\n");

        time_t send_time = time(NULL);
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        ssize_t sent = sendto(sockfd, cmd, sizeof(test_command_t), 0,(struct sockaddr*)uut_addr, sizeof(*uut_addr));
        if(sent < 0){
            perror("Sendto failed...");
            continue;
        }

        
        *from_len = sizeof(*from_addr);
        ssize_t received = recvfrom(sockfd, result, sizeof(*result), 0, (struct sockaddr*)from_addr, from_len);

        if(received < 0){
            perror("Recvfrom failed...\n");
            continue;
        }

        gettimeofday(&end_time, NULL);
        double duration = (end_time.tv_sec - start_time.tv_sec)+(end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        printf("\nTest-ID: %u\nResult: %s\nDuration: %.3f sec\n",
            result->test_id,
            result->result == TEST_SUCCESS ? "SUCCESS" : "FAILURE",
            duration);

        log_result(result->test_id, send_time, duration, result->result == TEST_SUCCESS);
    }
}

#endif