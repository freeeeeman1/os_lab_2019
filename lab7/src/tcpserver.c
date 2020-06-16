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

//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr
bool ConvertStringToUI64(const char *str, uint64_t *val);
int main(int argc, char *argv[]) {

	uint64_t buf_size = -1;
	uint64_t port = -1;

	while (true) {
		int current_optind = optind ? optind : 1;

		static struct option options[] = { {"buf_size", required_argument, 0, 0},
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
	if (buf_size == -1 || port == -1) {
		fprintf(stderr, "Using: %s --buf_size 1000 --port 5001 \n", argv[0]);
		return 1;
	}




  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  char buf[buf_size];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, buf_size)) > 0) {
      write(1, &buf, nread);
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
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
}}