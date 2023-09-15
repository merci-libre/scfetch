/*
 * scfetch version 0.4
 *
 * TODO before version 1.0 release:
 *
 * - get pci card information to print GPU model.
 * - give the prompt some color
 * - give the
 *
 *
 * */

// C-Standard libs
#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// System libs
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>

/* Developer notes:
 * These are structs that act as semi-global variables.
 * The reason they are in structs is for ease of access for
 * development so you know exactly which variable classes you
 * are calling throughout development.
 * */
struct directories {
  char homedirectory;
  char promptsdirectory;
  char prompt_path[512];
};
struct numbers {
  int files;
};

int findhomedir(struct directories *directory) {
  // config folder

  const char *homedir = getenv("HOME");

  // check if user has a home directory.
  DIR *homed = opendir(homedir);
  // printf("finding home folder...\n");

  if (homed == NULL) {
    perror("ERROR: Home directory does not exist \n");
    exit(1);
  }
  closedir(homed);
  strcpy(directory->prompt_path, homedir);

  return 0;
}

// check if `.config/sfetch/prompts` exists.
int find_prompts_path(struct directories *directory, int debug) {
  const char *path_to_directory = "/.config/shellrandomgreeter/prompts";
  strcat(directory->prompt_path, path_to_directory);

  DIR *prompts_exist = opendir(directory->prompt_path);

  if (prompts_exist == NULL) {
    perror("prompts folder does not exist");
    exit(1);
  }
  closedir(prompts_exist);
  if (debug == 1) {
    printf("prompts folder located at: %s\n", directory->prompt_path);
  }
  return 0;
}

/* Developer notes:
 * The function count_files() is used to determine the count of
 * the files within the user's config directory. It counts the number of
 * files to both use for logic gates, and to seed the randomfilegenerator()
 * function. The integer is stored in the numbers struct and can be called
 * anywhere within the program.
 * */

int count_files(struct directories *directory, struct numbers *integer,
                int debug, int l_flag) {

  int file_count = 0;
  if (debug == 1) {
    printf("counting the files in %s...\n", directory->prompt_path);
  }
  DIR *prompts_directory;
  prompts_directory = opendir(directory->prompt_path);
  struct dirent *entry;
  struct dirent *d_name;

  while ((entry = readdir(prompts_directory)) != NULL) {
    if (entry->d_type == DT_REG) {
      if (entry->d_name[0] != '.') {
        file_count++;
        if (debug == 1) {
          printf("index %d : %s\n", file_count, entry->d_name);
        }
      } else {
        file_count = file_count - 1;
      }
    }
  }
  closedir(prompts_directory);
  if (l_flag == 1) {
    exit(0);
  }
  if (debug == 1) {
    printf("files in %s: %d\n", directory->prompt_path, file_count);
  }
  integer->files = file_count;

  return 0;
}

/* take total number of prompts produced in count_files and create an index of
 * each file */

// create dynamic table
char **dynamic_table(int size) {
  char **filenames = (char **)malloc(size * sizeof(char *));
  for (int i = 0; i < size; i++) {
    filenames[i] = (char *)malloc(256 * sizeof(char));
  }
  return filenames;
}

// free allocated memory for files
void free_mem(char **filenames, int size) {
  for (int i = 0; i < size; i++) {
    free(filenames[i]);
  }
}

char *randomfilegenerator(char **filenames, int size, int debug,
                          int user_file) {
  if ((user_file == -1) && (debug == 1)) {
    printf("seeding %d to number generator...\n", size);
  }
  int random_index;
  if (size == 0) {
    perror("File is not accessible.\n");
    return NULL;
  }

  // check if user gave a predefined file.
  if ((user_file == -1) || (user_file > size)) {
    random_index = rand() % size;
  } else {
    random_index = user_file - 1;
  }

  if (debug == 1) {
    printf("number chosen for array is:  %d\n", random_index + 1);
  }
  return filenames[random_index];
}

char *choosefile(struct directories *directory, struct numbers *integer,
                 int user_file, int debug) {
  /* Developer notes:
   *  if there are no arguments given to option -f to print a specific file,
   *  this code is called to choose a random file from an
   *  array (dynamic table).
   * */
  int pf = integer->files;
  char *chosenfile;
  char **filenames = dynamic_table(pf); // allocate memory to array
  DIR *prompts_directory;
  prompts_directory = opendir(directory->prompt_path);
  int index = 0;
  struct dirent *entry;
  while ((entry = readdir(prompts_directory)) != NULL) {
    if (entry->d_type == DT_REG) {
      if (entry->d_name[0] != '.') {
        strcpy(filenames[index], entry->d_name);
        index++;
      }
    }
  }
  closedir(prompts_directory);
  srand(time(NULL));

  if (user_file == 0) {
    chosenfile = randomfilegenerator(filenames, pf, debug, 0);
  } else {
    chosenfile = randomfilegenerator(filenames, pf, debug, user_file);
  }
  return chosenfile;
}

/* SYSTEM INFORMATION */
int getCPUinfo() {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (cpuinfo == NULL) {
    perror("Error opening /proc/cpuinfo, uh-oh.");
    return 1;
  }

  char line[1024];
  while (fgets(line, sizeof(line), cpuinfo)) {
    if (strstr(line, "model name") != NULL) {
      char *modelname = strstr(line, ":");
      if (modelname != NULL) {
        modelname += 2;
        printf("CPU Model: %s", modelname);
        break;
      }
    }
  }
  fclose(cpuinfo);
  return 0;
}

/* does not work, keep commented out.
int getGPUinfo() {
  struct pci_access *pciaccess;
  struct pci_dev *pcidev;

  pciaccess = pci_alloc();
  pci_init(pciaccess);
  pci_scan_bus(pciaccess);

  for (pcidev = pciaccess->devices; pcidev; pcidev->next++) {
    pci_fill_info(pcidev, PCI_FILL_IDENT | PCI_FILL_CLASS);

    if (pcidev->device_class == 0x0300) {
      printf("GPU: 0x%x\n", pcidev->device_id);
    }
  }
  pci_cleanup(pciaccess);
  return 0;
}
*/

int uptime(long seconds_uptime) {
  /* TODO:
  Urgent
  - None, woohoo!
  Non-Urgent
   - switch over to switch/case format instead of else/if.
*/

  ulong seconds = seconds_uptime % 60;
  ulong minutes = (seconds_uptime % 3600) / 60;
  ulong hours = (seconds_uptime % 86400) / 3600;
  ulong days = seconds_uptime / 86400;

  // print uptime
  if (seconds_uptime < 60) {
    // seconds
    printf("Uptime: %ld seconds\n", seconds_uptime);
  } else if (seconds_uptime < 3600) {
    // minutes
    printf("Uptime: %ld minutes, %ld seconds\n", minutes, seconds);
  } else if (seconds_uptime < 86400) {
    // hours
    printf("Uptime: %ld hours, %ld minutes, %ld seconds\n", hours, minutes,
           seconds);
  } else if (seconds_uptime >= 86400) {
    printf("Uptime: %ld days, %ld hours, %ld minutes, %ld seconds\n", days,
           hours, minutes, seconds);
  }
  return 0;
};

/*
 * This is where everything is called into order
 * prints out in the following order:
 *
 * Welcome user@hostname!
 *
 * [Prompt goes here]
 *
 * System info:
 *
 * kernel information
 * system uptime
 * cpu information
 * memory used
 * memory available
 *
 * */

int printprompt(int debug, int user_file, int arguments) {
  char *user = getenv("USER");
  char hostname[1024];
  gethostname(hostname, 1024);

  // get username and host name for top of file:

  struct directories directory;
  struct numbers integers;
  struct sysinfo info;
  struct utsname k_info;
  findhomedir(&directory);
  find_prompts_path(&directory, debug);
  count_files(&directory, &integers, debug, 0);
  char *chosenfile = choosefile(&directory, &integers, user_file, debug);
  if (chosenfile != NULL) {
    strcat(directory.prompt_path, "/");
    strcat(directory.prompt_path, chosenfile);
  } else {
    printf("Files not found! Is the directory empty?\n");
  }

  FILE *fPointer;
  printf("\033[41mWelcome %s@%s!\n\n\033[0m", user, hostname);
  fPointer = fopen(directory.prompt_path, "r");
  if (fPointer != NULL) {
    char c;
    while ((c = fgetc(fPointer)) != EOF) {
      putchar(c);
    }
    fclose(fPointer);
  }

  // system calls:
  sysinfo(&info);
  uname(&k_info);

  printf("\n\033[41mSystem Info:\n\n\033[0m");
  // kernel information
  printf("Kernel version: %s %s\n", k_info.release, k_info.sysname);

  // uptime
  uptime(info.uptime);

  // CPU information
  getCPUinfo();

  // memory information
  printf("Memory Available: %ld GiB\n", info.freeram / 1024 / 1024 / 1024);
  printf("Total Memory Available: %ld GiB\n",
         info.totalram / 1024 / 1024 / 1024);

  if (debug == 1) {
    printf("\nDEBUGGING INFORMATION:\n");
    printf("file chosen: %s\n", directory.prompt_path);
    printf("arguments counted=%d\n", arguments - 1);
  }

  return 0;
}

void printhelp() {
  printf("usage: scfetch | scfetch [OPTIONS] | scfetch -f [index number]\n\n");
  printf("options:\n");
  printf("-f : When given an integer, -f prints out the prompt at the "
         "specified index.\n");
  printf("-h : Prints help menu.\n");
  printf("-l : Lists the files index array.\n");
  printf("-v : Be verbose.\n");
}

int main(int argc, char *argv[]) {
  int debug = 0;
  int user_file = -1;
  // if the command is run without any arguments, execute normally.
  // argument variables
  int option;
  int f_flag = 0;

  struct directories directory;
  struct numbers integers;
  findhomedir(&directory);
  find_prompts_path(&directory, debug);

  while ((option = getopt(argc, argv, "vhf:l")) != -1) {
    switch (option) {
    case 'h':
      printhelp();
      exit(0);
      break;
    case 'v':
      debug = 1;
      break;
    case 'f':
      if (f_flag) {
        printf("option -f detected, but no flag given.\n");
        exit(1);
      } else {
        f_flag++;
      }
      if ((strcmp(optarg, "0") == 0) || atoi(optarg) <= 0) {
        printf("You cannot print an empty/negative index.\n");
        exit(1);
      } else {
        user_file = atoi(optarg);
      }
      break;
    case 'l':
      count_files(&directory, &integers, 1, 1);
      exit(1);
      break;

    default:
      printf("Invalid Option\n");
      printhelp();
      exit(2);
    }
  }

  printprompt(debug, user_file, argc);
}

/* Hey y'all.
 * This project is currently incomplete.
 * I'm going to remake the entire old project in C.
 * However, implementing a solution to gathering system information
 * entirely in C is a lengthy process and is going to
 * require further work and research to be done.
 *
 * For the meanwhile, I'm going to continue working on this project
 * and leave it be for now.
 *
 * Going to upload it to Github for now just to have the project up!
 *
 * -westwardfishdme
 * */
