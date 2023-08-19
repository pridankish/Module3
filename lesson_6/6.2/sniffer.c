#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pcap.h>

void packet_handler_server(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    FILE *outfile = (FILE *)user_data;
    fwrite(packet, 1, pkthdr->len, outfile);
}

int main(){
    int sockfd;
    int clilen, n;
    char line[1000];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = 0;
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle_server = pcap_open_live("127.0.0.1", BUFSIZ, 1, 1000, errbuf);

    if (handle_server == NULL) {
        fprintf(stderr, "Couldn't open device: %s\n", errbuf);
        return 2;
    }

    FILE *outfile = fopen("server_packet_dump.dat", "ab");
    if (!outfile) {
        perror("Error opening output file");
        return 3;
    }

    while (1){
        clilen = sizeof(cliaddr);
        if ((n = recvfrom(sockfd, line, 999, 0,
        (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("%s\n", line);

        if (sendto(sockfd, line, strlen(line), 0, 
        (struct sockaddr *) &cliaddr, clilen) < 0){
            perror(NULL);
            close(sockfd);
            exit(EXIT_FAILURE);   
        }
        
        pcap_inject(handle_server, line, n);
        fwrite(line, 1, n, outfile);
    }

    fclose(outfile);
    close(sockfd);
    return 0;
}



