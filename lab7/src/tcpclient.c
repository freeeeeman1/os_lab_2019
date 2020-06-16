#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <getopt.h>
#include <errno.h>


//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

bool ConvertStringToUI64(const char *str, uint64_t *val);


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

	int fd;
	int nread;
	char buf[buf_size];


	struct sockaddr_in servaddr;
	if (argc < 3) {
		printf("Too few arguments \n");
		exit(1);
	}

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creating");
		exit(1);
	}

	memset(&servaddr, 0, SIZE);
	servaddr.sin_family = AF_INET;

	if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
		perror("bad address");
		exit(1);
	}

	servaddr.sin_port = htons(port);

	if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
		perror("connect");
		exit(1);
	}

	write(1, "Input message to send\n", 22);
	while ((nread = read(0, buf, buf_size)) > 0) {
		printf("..%d..", nread);
		if (write(fd, buf, nread) < 0) {
			printf("error write!");
			perror("write");
			exit(1);
		}
		
	}
	printf("..%i..", nread);

	close(fd);
	exit(0);
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