#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <getopt.h>
#include <errno.h>

bool ConvertStringToUI64(const char *str, uint64_t *val);
//#define SERV_PORT 20001
//#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {


	uint64_t buf_size = -1;
	char ip[255] = { '\0' };
	uint64_t port = -1;

	while (true) {
		int current_optind = optind ? optind : 1;

		static struct option options[] = { {"buf_size", required_argument, 0, 0},
										  {"ip", required_argument, 0, 0},
										  {"port", required_argument, 0, 0},
										  {0, 0, 0, 0} };

		int option_index = 0;
		int c = getopt_long(argc, argv, "", options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 0: {
			switch (option_index) {
			case 0:
				ConvertStringToUI64(optarg, &buf_size);
				if (buf_size < 0) {
					printf("K %llu < 0!\n", buf_size);
					exit(1);
				}
				break;
			case 1:
				memcpy(ip, optarg, strlen(optarg));
				//	printf("\nip-%s\n", ip);

				break;
			case 2:
				ConvertStringToUI64(optarg, &port);
				if (port < 0) {
					printf("Mod %llu < 0!\n", port);
					exit(1);
				}
				break;
			default:
				printf("Index %d is out of options\n", option_index);
			}
		} break;

		case '?':
			printf("Arguments error\n");
			break;
		default:
			printf(stderr, "getopt returned character code 0%o?\n", c);
		}
	}
	if (buf_size == -1 || port == -1 || !strlen(ip)) {
		fprintf(stderr, "Using: %s --buf_size 1000 --ip 127.0.0.1 --port 5001 \n", argv[0]);
		return 1;
	}


  int sockfd, n;
  char sendline[buf_size], recvline[buf_size + 1];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if (argc <3) {
    printf("usage: client <IPaddress of server>\n");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, buf_size)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (SADDR *)&servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }

    if (recvfrom(sockfd, recvline, buf_size, 0, NULL, NULL) == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
	char *end = NULL;
	unsigned long long i = strtoull(str, &end, 10);
	if (errno == ERANGE) {
		fprintf(stderr, "Out of uint64_t range: %s\n", str);
		return false;
	}

	if (errno != 0)
		return false;

	*val = i;
	return true;
}