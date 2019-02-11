#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define N_PAGES 3

void p_s(void)
{
  int fd;
  char buff[1];
  sleep(3);
  fd = open("/sys/kernel/mm/ksm/pages_sharing", O_RDONLY);
  read(fd, &buff, 1);
  printf("Sharing pages: %d\n", atoi(buff));
  close(fd);
}

void main(int argc, char *argv[])
{
  int i;
  // size_t p_size = sysconf(_SC_PAGE_SIZE);
  size_t p_size = 16384;

  printf("Size %lu\n", p_size);

  void **pages = (void **)calloc(N_PAGES, sizeof(void *));

  // I create 3 pages and fill them with zeroes
  for (i = 0; i < N_PAGES; i++)
  {
    // ((pages[i] = mmap(NULL, p_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) && (pages[i])) ? memset(pages[i], 0, p_size) && madvise(pages[i], p_size, MADV_MERGEABLE) : exit(-1);
    pages[i] = mmap(NULL, p_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (pages[i])
    {
      memset(pages[i], 0, p_size);
      madvise(pages[i], p_size, MADV_MERGEABLE);

      printf("Pages content %d : %p\n", i, pages[i]);
    }
    else
    {
      exit(-1);
    }

    // ksm_madvise();
    // printf("Pages content %d : %p\n", i, pages[i]);
  }

  p_s();

  if (argc > 1)
  {
    if (argv[1][0] == 'y')
    {

      printf("modifying pages contents \n");

      // Then I modify the pages to some "random" value
      for (i = 0; i < N_PAGES; i++)
      {
        memset(pages[i], i + 1337, 1);
      };
    }
  }

  while (1)
  {
    p_s();
  }
}